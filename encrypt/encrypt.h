/*********************************************************************************
  *Copyright(C),2016-2018,yzfcer@163.com
  *FileName:  
  *Author:      Zhou Jiangcun
  *Version:     1.0
  *Date:        2016/11/04
  *Description:  
  *Others:  
  *History:  
     1.Date:
       Author:
       Modification:
**********************************************************************************/
#ifndef ENCRYPT_H__
#define ENCRYPT_H__
#include "boot_type.h"

#define ENCRYPT_KEY {0x12,0x25,0x38,0x54,0x85,0xa5,0x7c,0x96,0x34,0x57,0xf2,0x39,0x0a,0x83,0x98,0x65}
int encrypt_data(int crypttype,uint8_t *data,int lenth);
int decrypt_data(int crypttype,uint8_t *data,int lenth);

#endif 
