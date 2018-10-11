/*********************************************************************************
  *Copyright(C),2017-2020,yzfcer@163.com
  *FileName:  
  *Author:      Jason Zhou
  *Version:     1.0
  *Date:        2017/04/08
  *Description:  
  *Others:  
  *History:  
     1.Date:
       Author:
       Modification:
**********************************************************************************/
#ifndef BOOT_SHARE_PARAM_H__
#define BOOT_SHARE_PARAM_H__
#include "wind_type.h"
#ifdef __cplusplus
extern "C" {
#endif

#define SHARE_PARAM_MAGIC 0x52d360a5
#define SHARE_BASE 0x2002F000
#define SHARE_LENTH 0x1000
typedef struct 
{
    w_uint32_t exist;
    w_uint32_t isOK;
}hw_status_s;

//����������
typedef struct
{
    w_uint32_t flag;//������־
    w_uint32_t mem_type;//��������Ľ�������
    w_uint32_t addr;//��������ĵ�ַ
    w_uint32_t size;//��������Ŀռ��С
    w_uint32_t datalen;//������������ݳ���
}upgrade_info_s;

//�����ռ������
typedef struct
{
    w_uint32_t addr;//�����ռ�ĵ�ַ
    w_uint32_t size;//�����ռ�ĳ���
    w_uint32_t mem_type;//�����ռ������
    w_uint32_t pad;
}sysparam_part_s;


w_err_t sp_init_share_param(void);

w_err_t sp_set_upgrade_param(upgrade_info_s *part);

w_err_t sp_get_upgrade_param(upgrade_info_s *part);

w_err_t sp_set_sysparam_param(sysparam_part_s *sysparam);

w_err_t sp_get_sysparam_param(sysparam_part_s *sysparam);


#ifdef __cplusplus
}
#endif

#endif
