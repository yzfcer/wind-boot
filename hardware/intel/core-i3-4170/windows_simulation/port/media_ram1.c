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
#include "boot_media.h"
#include "wind_string.h"
#include "wind_debug.h"
//Ӳ��RAM�����ͻ���ַ�Ϳռ��С����
#define RAM1_BASE 0x20000000
#define RAM1_SIZE 0x400000
#define RAM1_BLKSIZE 512
w_uint8_t g_ram1[RAM1_SIZE];


static w_err_t ram1_init(w_media_s *media)
{
    wind_memset(g_ram1,0,RAM1_SIZE);
    return W_ERR_OK;
}
static w_int32_t ram1_read_blk(w_media_s *media,w_uint32_t addr,w_uint8_t *data,w_int32_t blkcnt)
{
    wind_memcpy(data,&g_ram1[addr],blkcnt * RAM1_BLKSIZE);
    return blkcnt;
}
static w_int32_t ram1_write_blk(w_media_s *media,w_uint32_t addr,w_uint8_t *data,w_int32_t blkcnt)
{
    wind_memcpy(&g_ram1[addr],data,blkcnt * RAM1_BLKSIZE);
    return blkcnt;
}

static w_int32_t ram1_erase_blk(w_media_s *media,w_uint32_t addr,w_int32_t blkcnt)
{
    wind_memset(&g_ram1[addr],0,blkcnt * RAM1_BLKSIZE);
    return blkcnt;
}
static w_err_t ram1_erase_all(w_media_s *media)
{
    wind_memset(g_ram1,0,RAM1_SIZE);
    return W_ERR_OK;
}
static w_media_ops_s ops = 
{
    ram1_init,
    ram1_read_blk,
    ram1_write_blk,
    ram1_erase_blk,
    ram1_erase_all
};

w_media_s media_ram1 = 
{
    "ram1",
    IRAM,
    0,
    RAM1_SIZE,
    RAM1_BLKSIZE,
    0,
    &ops
};


