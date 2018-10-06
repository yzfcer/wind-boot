/*********************************************************************************
  *Copyright(C),2016-2018,yzfcer@163.com
  *FileName:  
  *Author:      Jason Zhou
  *Version:     1.0
  *Date:        2016/10/08
  *Description:  
  *Others:  
  *History:  
     1.Date:
       Author:
       Modification:
**********************************************************************************/
#include <stdio.h>
#include "boot_media.h"
#include "wind_string.h"
#include "wind_debug.h"
//Ӳ��RAM�����ͻ���ַ�Ϳռ��С����
#define ROM2_SIZE 0x400000
#define ROM2_BLKSIZE 512
w_uint8_t g_rom2[ROM2_SIZE];

static void flush_rom_file(void)
{
    FILE *file;
    file = fopen("rom2.bin","wb+");
    fwrite(g_rom2,1,ROM2_SIZE,file);
    fclose(file);
}

static void read_rom_file(void)
{
    FILE *file;
    file = fopen("rom2.bin","rb");
    fread(g_rom2,1,ROM2_SIZE,file);
    fclose(file);
}


static w_err_t rom2_init(w_media_s *media)
{
    
    FILE *file;
    file = fopen("rom1.bin","rb");
    if(file == W_NULL)
    {
        file = fopen("rom1.bin","rb");
        WIND_ASSERT_RETURN(file != 0,W_ERR_FAIL);
    }
    fclose(file);
    wind_memset(g_rom2,0,ROM2_BLKSIZE);
    return W_ERR_OK;
}
static w_int32_t rom2_read_blk(w_media_s *media,w_uint32_t addr,w_uint8_t *data,w_int32_t blkcnt)
{
    read_rom_file();
    wind_memcpy(data,&g_rom2[addr],blkcnt * ROM2_BLKSIZE);
    return blkcnt;
}
static w_int32_t rom2_write_blk(w_media_s *media,w_uint32_t addr,w_uint8_t *data,w_int32_t blkcnt)
{
    wind_memcpy(&g_rom2[addr],data,blkcnt * ROM2_BLKSIZE);
    flush_rom_file();
    return blkcnt;
}

static w_int32_t rom2_erase_blk(w_media_s *media,w_uint32_t addr,w_int32_t blkcnt)
{
    wind_memset(&g_rom2[addr],0,blkcnt * ROM2_BLKSIZE);
    read_rom_file();
    return blkcnt;
}
static w_err_t rom2_erase_all(w_media_s *media)
{
    wind_memset(g_rom2,0,ROM2_BLKSIZE);
    read_rom_file();
    return W_ERR_OK;
}
static w_media_ops_s ops = 
{
    rom2_init,
    rom2_read_blk,
    rom2_write_blk,
    rom2_erase_blk,
    rom2_erase_all
};

w_media_s media_rom2 = 
{
    "rom2",
    IRAM,
    0,
    ROM2_SIZE,
    ROM2_BLKSIZE,
    0,
    &ops
};
