#include <stdio.h>
#include <stdlib.h>
#include "pds.h"
#include "bst.h"
#include <errno.h>
#include <string.h>
struct PDS_RepoInfo repo_handle;
int pds_open( char *repo_name, int rec_size )
{
    char repofilename[30];
    strcpy(repofilename, repo_name);
    strcat(repofilename, ".dat");
    if(repo_handle.repo_status==PDS_REPO_OPEN)
        return PDS_REPO_ALREADY_OPEN;
    
    repo_handle.pds_data_fp=(FILE *)fopen(repofilename, "rb+");
    if(repo_handle.pds_data_fp==NULL)
    {
        perror(repofilename);
    }
    strcpy(repo_handle.pds_name, repofilename);
    repo_handle.rec_size=rec_size;
    repo_handle.pds_bst=NULL;
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
int get_rec_by_key(int key, void *rec)
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
int pds_close()
{
    strcpy(repo_handle.pds_name, "");
    fclose(repo_handle.pds_data_fp);
    bst_free(repo_handle.pds_bst);
    repo_handle.repo_status=PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}