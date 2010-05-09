
#ifndef	_HAHA_FS_H_
#define	_HAHA_FS_H_

/**
 * @struct dev_drv_map fs.h "include/sys/fs.h"
 * @brief  The Device_nr.\ - Driver_nr.\ MAP.
 *
 *  \dot
 *  digraph DD_MAP {
 *    graph[rankdir=LR];
 *    node [shape=record, fontname=Helvetica];
 *    b [ label="Device Nr."];
 *    c [ label="Driver (the task)"];
 *    b -> c [ label="DD_MAP", fontcolor=blue, URL="\ref DD_MAP", arrowhead="open", style="dashed" ];
 *  }
 *  \enddot
 */
struct dev_drv_map {
	int driver_nr; /**< The proc nr.\ of the device driver. */
};

/**
 * @def   MAGIC_V1
 * @brief Magic number of FS v1.0
 */
#define	MAGIC_V1	0x111

/**
 * @struct super_block fs.h "include/fs.h"
 * @brief  The 2nd sector of the FS
 *
 * Remember to change SUPER_BLOCK_SIZE if the members are changed.
 *
 * @attention Remember to change boot/include/load.inc::SB_* if the members
 *            are changed.
 *
 * \dot
 * digraph super_block {
 *         node [shape=plaintext];
 * 
 *         sb [label=<<TABLE CELLPADDING="0" CELLSPACING="0" BORDER="0">
 *                 <TR>
 *                         <TD HEIGHT="20" WIDTH="70" ALIGN="right" VALIGN="top" BORDER="0">Sector #&nbsp;</TD>
 *                         <TD HEIGHT="20" WIDTH="90" ALIGN="left" VALIGN="top" BORDER="0">0</TD>
 *                         <TD HEIGHT="20" WIDTH="90" ALIGN="left" VALIGN="top" BORDER="0">1</TD>
 *                         <TD HEIGHT="20" WIDTH="290" ALIGN="center" VALIGN="top" BORDER="0">...</TD>
 *                         <TD HEIGHT="20" WIDTH="10" ALIGN="right" BORDER="0"></TD>
 *                 </TR>
 *                 <TR>
 *                         <TD HEIGHT="30" WIDTH="70" ALIGN="right" BORDER="0"></TD>
 *                         <TD HEIGHT="30" WIDTH="90" ALIGN="center" BORDER="1" BGCOLOR="white">Boot Sector</TD>
 *                         <TD HEIGHT="30" WIDTH="90" ALIGN="center" BORDER="1" BGCOLOR="grey">Super Block</TD>
 *                         <TD HEIGHT="30" WIDTH="290" ALIGN="center" BORDER="1" BGCOLOR="white">...</TD>
 *                         <TD HEIGHT="30" WIDTH="10" ALIGN="right" BORDER="0"></TD>
 *                 </TR>
 *         </TABLE>>];
 * }
 * \enddot
 */
struct ext3_super_block{
/*00*/ u32 s_inodes_count;      /* inodes 计数 */
       u32 s_blocks_count;      /* blocks 计数 */
       u32 s_r_blocks_count;    /* 保留的 blocks 计数 */
       u32 s_free_blocks_count; /* 空闲的 blocks 计数 */
/*10*/ u32 s_free_inodes_count; /* 空闲的 inodes 计数 */
       u32 s_first_data_block;  /* 第一个数据 block */
       u32 s_log_block_size;    /* block 的大小 */
       u32 s_log_frag_size;     /* 可以忽略 */
/*20*/ u32 s_blocks_per_group;  /* 每 block group 的 block 数量 */
       u32 s_frags_per_group;   /* 可以忽略 */
       u32 s_inodes_per_group;  /* 每 block group 的 inode 数量 */
       u32 s_mtime;             /* Mount time */
/*30*/ u32 s_wtime;             /* Write time */
       u16 s_mnt_count;         /* Mount count */
       u16 s_max_mnt_count;     /* Maximal mount count */
       u16 s_magic;             /* Magic 签名 */
       u16 s_state;             /* File system state */
       u16 s_errors;            /* Behaviour when detecting errors */
       u16 s_minor_rev_level;   /* minor revision level */
/*40*/ u32 s_lastcheck;         /* time of last check */
       u32 s_checkinterval;     /* max. time between checks */
       u32 s_creator_os;        /* 可以忽略 */
       u32 s_rev_level;         /* Revision level */
/*50*/ u16 s_def_resuid;        /* Default uid for reserved blocks */
       u16 s_def_resgid;        /* Default gid for reserved blocks */
       u32 s_first_ino;         /* First non-reserved inode */
       u16 s_inode_size;        /* size of inode structure */
       u16 s_block_group_nr;    /* block group # of this superblock */
       u32 s_feature_compat;    /* compatible feature set */
/*60*/ u32 s_feature_incompat;  /* incompatible feature set */
       u32 s_feature_ro_compat; /* readonly-compatible feature set */
/*68*/ u8  s_uuid[16];          /* 128-bit uuid for volume */
/*78*/ char  s_volume_name[16];   /* volume name */
/*88*/ char  s_last_mounted[64];  /* directory where last mounted */
/*C8*/ u32 s_algorithm_usage_bitmap; /* 可以忽略 */
       u8  s_prealloc_blocks;        /* 可以忽略 */
       u8  s_prealloc_dir_blocks;    /* 可以忽略 */
       u16 s_padding1;               /* 可以忽略 */
/*D0*/ u8  s_journal_uuid[16]; /* uuid of journal superblock */
/*E0*/ u32 s_journal_inum;     /* 日志文件的 inode 号数 */
       u32 s_journal_dev;      /* 日志文件的设备号 */
       u32 s_last_orphan;      /* start of list of inodes to delete */
/*EC*/ u32 s_reserved[197];    /* 可以忽略 */

};

struct ext3_group_desc
{
 u32 bg_block_bitmap;      /* block 指针指向 block bitmap */
 u32 bg_inode_bitmap;      /* block 指针指向 inode bitmap */
 u32 bg_inode_table;       /* block 指针指向 inodes table */
 u16 bg_free_blocks_count; /* 空闲的 blocks 计数 */
 u16 bg_free_inodes_count; /* 空闲的 inodes 计数 */
 u16 bg_used_dirs_count;   /* 目录计数 */
 u16 bg_pad;               /* 可以忽略 */
 u32 bg_reserved[3];       /* 可以忽略 */
};
#define EXT3_N_BLOCKS 15
struct ext3_inode {
 u16 i_mode;    /* File mode */
 u16 i_uid;     /* Low 16 bits of Owner Uid */
 u32 i_size;    /* 文件大小，单位是 byte */
 u32 i_atime;   /* Access time */
 u32 i_ctime;   /* Creation time */
 u32 i_mtime;   /* Modification time */
 u32 i_dtime;   /* Deletion Time */
 u16 i_gid;     /* Low 16 bits of Group Id */
 u16 i_links_count;          /* Links count */
 u32 i_blocks;               /* blocks 计数 */
 u32 i_flags;                /* File flags */
 u32 l_i_reserved1;          /* 可以忽略 */
 u32 i_block[EXT3_N_BLOCKS]; /* 一组 block 指针 */
 u32 i_generation;           /* 可以忽略 */
 u32 i_file_acl;             /* 可以忽略 */
 u32 i_dir_acl;              /* 可以忽略 */
 u32 i_faddr;                /* 可以忽略 */
 u8  l_i_frag;               /* 可以忽略 */
 u8  l_i_fsize;              /* 可以忽略 */
 u16 i_pad1;                 /* 可以忽略 */
 u16 l_i_uid_high;           /* 可以忽略 */
 u16 l_i_gid_high;           /* 可以忽略 */
 u32 l_i_reserved2;          /* 可以忽略 */
};
#define EXT3_NAME_LEN 0xF8
struct ext3_dir_entry_2 {
 u32 inode;    /* Inode 号数 */
 u16 rec_len;  /* Directory entry length */
 u8  name_len; /* Name length */
 u8  file_type;
 char  name[EXT3_NAME_LEN]; /* File name 0xf8*/
};
struct super_block {
	u32	magic;		  /**< Magic number */
	u32	nr_inodes;	  /**< How many inodes */
	u32	nr_sects;	  /**< How many sectors (including bit maps) */
	u32	nr_imap_sects;	  /**< How many inode-map sectors */
	u32	nr_smap_sects;	  /**< How many sector-map sectors */
	u32	n_1st_sect;	  /**< Number of the 1st data sector */

	u32	nr_inode_sects;   /**< How many inode sectors */
	u32	root_inode;       /**< Inode nr of root directory */
	u32	inode_size;       /**< INODE_SIZE */
	u32	inode_isize_off;  /**< Offset of `struct inode::i_size' */
	u32	inode_start_off;  /**< Offset of `struct inode::i_start_sect' */
	u32	dir_ent_size;     /**< DIR_ENTRY_SIZE */
	u32	dir_ent_inode_off;/**< Offset of `struct dir_entry::inode_nr' */
	u32	dir_ent_fname_off;/**< Offset of `struct dir_entry::name' */

	/*
	 * the following item(s) are only present in memory
	 */
	int	sb_dev; 	/**< the super block's home device */
};

/**
 * @def   SUPER_BLK_MAGIC_V1
 * @brief Magic number of super block, version 1.
 * @attention It must correspond with boot/include/load.h::SB_MAGIC_V1
 */
#define	SUPER_BLK_MAGIC_V1		0x111

/**
 * @def   SUPER_BLOCK_SIZE
 * @brief The size of super block \b in \b the \b device.
 *
 * Note that this is the size of the struct in the device, \b NOT in memory.
 * The size in memory is larger because of some more members.
 */
#define	SUPER_BLOCK_SIZE	56

/**
 * @struct inode
 * @brief  i-node
 *
 * The file, currently, have tree valid attributes only:
 *   - size
 *   - start_sect
 *   - nr_sects
 *
 * The \c start_sect and\c nr_sects locate the file in the device,
 * and the size show how many bytes is used.
 * If <tt> size < (nr_sects * SECTOR_SIZE) </tt>, the rest bytes
 * are wasted and reserved for later writing.
 *
 * \b NOTE: Remember to change INODE_SIZE if the members are changed
 */
struct inode {
	u32	i_mode;		/**< Accsess mode. Unused currently */
	u32	i_size;		/**< File size */
	u32	i_start_sect;	/**< The first sector of the data */
	u32	i_nr_sects;	/**< How many sectors the file occupies */
	u8	_unused[16];	/**< Stuff for alignment */

	/* the following items are only present in memory */
	int	i_dev;
	int	i_cnt;		/**< How many procs share this inode  */
	int	i_num;		/**< inode nr.  */
};

/**
 * @def   INODE_SIZE
 * @brief The size of i-node stored \b in \b the \b device.
 *
 * Note that this is the size of the struct in the device, \b NOT in memory.
 * The size in memory is larger because of some more members.
 */
#define	INODE_SIZE	32

/**
 * @struct file_desc
 * @brief  File Descriptor
 */
struct file_desc {
	int		fd_mode;	/**< R or W */
	int		fd_pos;		/**< Current position for R/W. */
	struct inode*	fd_inode;	/**< Ptr to the i-node */
};


/**
 * Since all invocations of `rw_sector()' in FS look similar (most of the
 * params are the same), we use this macro to make code more readable.
 *
 * Before I wrote this macro, I found almost every rw_sector invocation
 * line matchs this emacs-style regex:
 * `rw_sector(\([-a-zA-Z0-9_>\ \*()+.]+,\)\{3\}\ *SECTOR_SIZE,\ *TASK_FS,\ *fsbuf)'
 */
#define RD_SECT(dev,sect_nr) rw_sector(DEV_READ, \
				       dev,				\
				       (sect_nr) * SECTOR_SIZE,		\
				       SECTOR_SIZE, /* read one sector */ \
				       TASK_FS,				\
				       fsbuf);
#define WR_SECT(dev,sect_nr) rw_sector(DEV_WRITE, \
				       dev,				\
				       (sect_nr) * SECTOR_SIZE,		\
				       SECTOR_SIZE, /* write one sector */ \
				       TASK_FS,				\
				       fsbuf);

	
#endif /* _ORANGES_FS_H_ */
