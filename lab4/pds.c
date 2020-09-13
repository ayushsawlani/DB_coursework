#include <stdio.h>
#include <stdlib.h>
#include "pds.h"
#include "bst.h"
#include <errno.h>
#include <string.h>
struct PDS_RepoInfo repo_handle;
int pds_open( char *repo_name, int rec_size )
{
    char repofilename[30],  repondxname[30];
    strcpy(repofilename, repo_name);
    strcat(repofilename, ".dat");
    strcpy(repondxname, repo_name);
    strcat(repondxname, ".ndx");
    if(repo_handle.repo_status==PDS_REPO_OPEN)
        return PDS_REPO_ALREADY_OPEN;
    
    repo_handle.pds_data_fp=(FILE *)fopen(repofilename, "rb+");
    if(repo_handle.pds_data_fp==NULL)
    {
        perror(repofilename);
    }
    repo_handle.pds_ndx_fp=(FILE *)(fopen(repondxname, "rb+"));
    if(repo_handle.pds_ndx_fp==NULL)
    {
        perror(repondxname);
    }
    strcpy(repo_handle.pds_name, repo_name);
    repo_handle.rec_size=rec_size;
    repo_handle.pds_bst=NULL;
    struct PDS_NdxInfo *entry=(struct PDS_NdxInfo *)(malloc(sizeof(struct PDS_NdxInfo)));
    while(fread(entry, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp)==1)
    {
        bst_add_node(&repo_handle.pds_bst, entry->key, entry);
        //printf("%d %d\n", entry->offset, entry->key);
        entry=(struct PDS_NdxInfo *)(malloc(sizeof(struct PDS_NdxInfo)));
    }//*/
    fclose(repo_handle.pds_ndx_fp);
    free(entry);
    repo_handle.repo_status=PDS_REPO_OPEN;
    return PDS_SUCCESS;
}
int put_rec_by_key( int key, void *rec )
{
    int offset;
    fseek(repo_handle.pds_data_fp, 0, SEEK_END);
    offset=ftell(repo_handle.pds_data_fp);
    struct PDS_NdxInfo *entry=(struct PDS_NdxInfo *)(malloc(sizeof(struct PDS_NdxInfo)));
    entry->key=key;
    entry->offset=offset;
    int status=bst_add_node(&repo_handle.pds_bst, key, entry);
    if(status==BST_SUCCESS)
    {
        fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
        return PDS_SUCCESS;
    }
    else
    {
        free(entry);
        return PDS_ADD_FAILED;
    }
}
int get_rec_by_ndx_key(int key, void *rec)
{
    struct BST_Node *node=bst_search(repo_handle.pds_bst, key);
    if(node==NULL)
    {
        return PDS_REC_NOT_FOUND;
    }
    else
    {
        struct PDS_NdxInfo *entry=(struct PDS_NdxInfo *)node->data;
        int offset=entry->offset;
        fseek(repo_handle.pds_data_fp, offset, SEEK_SET);
        fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
        return PDS_SUCCESS;
    }
}
void pre_order(struct BST_Node *node)
{
    if(node!=NULL)
    {
        pre_order((node)->left_child);
        fwrite(node->data, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp);
        pre_order((node)->right_child);
    }
}
int pds_close()
{
    char repondxname[30];
    strcpy(repondxname, repo_handle.pds_name);
    strcat(repondxname, ".ndx");
    //printf("%s\n", repondxname);
    repo_handle.pds_ndx_fp=(FILE*)(fopen(repondxname, "wb"));
    pre_order(repo_handle.pds_bst);
    fclose(repo_handle.pds_ndx_fp);
    strcpy(repo_handle.pds_name, "");
    fclose(repo_handle.pds_data_fp);
    bst_free(repo_handle.pds_bst);
    repo_handle.repo_status=PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}
int get_rec_by_non_ndx_key(
    void *key, /* The search key */
    void *rec, /* The output record */
    int (*matcher)(void *rec, void *key), /*Function pointer for matching*/
    int *io_count /* Count of the number of records read */
    )
{
    fseek(repo_handle.pds_data_fp, 0, SEEK_SET);
    while((fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp)==1))
    {
        (*io_count)++;
        if(matcher(rec, key)==0)
            break;
    }
    if(matcher(rec, key)==0)
    {
        return PDS_SUCCESS;
    }
    else
    {
        return PDS_REC_NOT_FOUND;
    }
}