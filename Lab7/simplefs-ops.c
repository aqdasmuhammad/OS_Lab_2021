#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; // Array for storing opened files

int simplefs_create(char *filename){
    /*
	    Create file with name `filename` from disk
	*/
	struct inode_t *inodeptr =(struct inode_t*)malloc(sizeof(struct inode_t));
	i=simplefs_allocInode();
	k=0;
	for (j=0; j<8; j++){
		simplefs_readInode(int j, struct inode_t *inodeptr);
		if (filename == inodeptr -> name){
			k++;
		}
		else {
			continue;
		}
	}
	if (k == 0){
		d1=simplefs_allocDataBlock();
		d2=simplefs_allocDataBlock();
		d3=simplefs_allocDataBlock();
		d4=simplefs_allocDataBlock();
		inodeptr -> status = 1;
		inodeptr -> name = filename;
		inodeptr -> file_size = 256;
		inodeptr -> direct_blocks[4]=[d1, d2, d3, d4];
		simplefs_writeInode(i, inodeptr);
		return i;
	}
    return -1;
}


void simplefs_delete(char *filename){
    /*
	    delete file with name `filename` from disk
	*/
	struct inode_t *inodeptr =(struct inode_t*)malloc(sizeof(struct inode_t));
	for (i=0; i<8; i++){
		simplefs_readInode(int i, struct inode_t *inodeptr);
		if (filename == inodeptr -> name){
			for (j=0; j<4; j++){
				simplefs_freeDataBlock(int direct_blocks[j])
			}
			simplefs_freeInode(int i);
			break
		}
	}
}

int simplefs_open(char *filename){
    /*
	    open file with name `filename`
	*/
		struct inode_t *inodeptr =(struct inode_t*)malloc(sizeof(struct inode_t));
		simplefs_readInode(int i, struct inode_t *inodeptr);
		for (i=0; i<8; i++){
			simplefs_readInode(int i, struct inode_t *inodeptr);
			if (filename == inodeptr -> name){
				if (filehandler_t -> inode_number == i){
					filehandler_t -> 
				}
			}                                                           
			
		}
    return -1;
}

void simplefs_close(int file_handle){
    /*
	    close file pointed by `file_handle`
	*/

}

int simplefs_read(int file_handle, char *buf, int nbytes){
    /*
	    read `nbytes` of data into `buf` from file pointed by `file_handle` starting at current offset
	*/
    return -1;
}


int simplefs_write(int file_handle, char *buf, int nbytes){
    /*
	    write `nbytes` of data from `buf` to file pointed by `file_handle` starting at current offset
	*/
    return -1;
}


int simplefs_seek(int file_handle, int nseek){
    /*
	   increase `file_handle` offset by `nseek`
	*/
    return -1;
}
