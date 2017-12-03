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
#include "mem_map.h"
#include "boot_port.h"
#include "share_param.h"
#include "sys_debug.h"
#include "boot_param.h"
#include "mem_driver.h"
#include "wind_string.h"
#ifdef __cplusplus
extern "C" {
#endif

extern part_s g_region_map[];
w_uint32_t mem_map_share_addr(void)
{
    part_s *entry = part_get_inst_name("share");
	return entry->addr;
}

char *memtype_name(w_int16_t type)
{
    if(type == MEM_TYPE_RAM)
        return "RAM";
    else if(type == MEM_TYPE_ROM)
        return "ROM";
    return "UNKNOWN";
}


region_s *mem_map_get_reg(char *name)
{
    w_int32_t i;
    region_s *reg = (region_s *)boot_param_mem_map();
    w_int32_t count = mem_map_get_reg_count();
    for(i = 0;i < count;i ++)
    {
        if(wind_strcmp(reg[i].name,name) == 0)
            return reg;
    }
    sys_error("find region %s failed.",name);
    return (region_s *)NULL;
}

region_s *mem_map_get_list(void)
{
    boot_param_s *bp = boot_param_instance();
    return (region_s*)(sizeof(boot_param_s)+(w_uint32_t)bp);
    
}

w_int32_t mem_map_get_reg_count(void)
{
    boot_param_s *bp = boot_param_instance();
    return bp->reg_count;
}






static void copy_reg_name(char *dest,char *src)
{
    int len = wind_strlen(dest);
    len = len > PART_NAME_LEN-1?PART_NAME_LEN-1:len;
    wind_memcpy(dest,src,len);
}


w_int32_t mem_map_reset(region_s *map)
{
    w_int32_t i;
    region_s *rg = map;
    part_s *entry = part_get_list();
    w_int32_t count = part_get_count();
    for(i = 0;i < count;i ++)
    {
        copy_reg_name(rg[i].name,entry->name);
        rg[i].memidx = entry->memidx;
        rg[i].addr = entry->addr;
        rg[i].size = entry->size;
        rg[i].datalen = 0;
        rg[i].crc = 0xffffff;
        rg[i].status = MEM_NULL;
    }
    return 0;
}







#if 0
w_int32_t check_probuf_and_running(region_s *probuf,region_s *run)
{
    if(probuf->type != run->type)
        return 0;
    if(0 == do_check_conflict(probuf,run))
        return 0;
    if(probuf->addr >= run->addr)
        return 0;
    return -1;
}
#endif
w_int32_t mem_map_check(void)
{
    sys_notice("begin to check momery map params...");

    if(part_check_conflict())
    {
        sys_warn("check ram conflict error.");
        return -1;
    }
#if 0
    if(check_rom_type(&map->rom))
    {
        sys_warn("check rom type error.");
        return -1;
    }
    if(check_ram_type(&map->ram))
    {
        sys_warn("check ram type error.");
        return -1;
    }
    if(check_run_type(&map->run))
    {
        sys_warn("check running space type error.");
        return -1;
    }
    if(check_probuf_and_running(&map->ram.load_buffer,&map->run.flash))
    {
        sys_warn("program buffer and running space conflict.");
        return -1;
    }
#endif
    sys_notice("check momery map params OK.");
    return 0;   
}



void mem_map_print_status(void)
{
#define REGION_FORMAT1 "%-15s%-8d0x%-10x0x%-9x0x%-9x%-9s%4d%%\r\n" 
#define REGION_PARAM1(reg) (reg).name,(reg).memidx,(reg).addr,(reg).size,\
                (reg).datalen,(reg).name,(reg).size?((reg).datalen*100)/(reg).size:0
    w_int32_t i;
    w_int32_t count = mem_map_get_reg_count();
    region_s *reg = mem_map_get_list();

    wind_printf("%-15s%-8s%-12s%-11s%-11s%-9s%-8s\r\n","region","memidx","addr","size","datalen","type","usage");
    for(i = 0;i < count;i ++)
    {
        wind_printf(REGION_FORMAT1,REGION_PARAM1(reg[i]));
    }
}

void mem_map_copy_info(region_s *src,region_s *dest)
{
	wind_strcpy(dest->name,src->name);
    dest->type = src->type;
    dest->memidx = src->memidx;
    dest->addr = src->addr;
    dest->size = src->size;
    dest->datalen = src->datalen;
    dest->crc = src->crc;
    dest->status = src->status;
}
static void print_copy_percents(w_int32_t numerator, w_int32_t denominator,w_int32_t del)
{
    if(del)
        wind_printf("%c%c%c%c",8,8,8,8);
    wind_printf("%3d%%",numerator*100/denominator);
        feed_watchdog();
}

w_int32_t mem_map_copy_data(region_s *src,region_s *dest)
{
    w_int32_t i,j,len,blocks,times;
    w_uint32_t addr;
    w_uint8_t *buff = get_block_buffer();

    if(0 >= src->datalen)
        return 0;
    if(dest->size < src->datalen)
    {
        sys_warn("space is NOT enough.");
        return -1;
    }
    sys_notice("copy data from \"%s\" to \"%s\" lenth %d.",
                src->name,dest->name,src->datalen);
    sys_debug("source type %s,addr 0x%x,lenth %d dest type,%s,addr 0x%x,lenth %d.",
                memtype_name(src->type),src->addr,src->datalen,
                memtype_name(dest->type),dest->addr,dest->size);
    
    blocks = (src->datalen + BLOCK_SIZE - 1) / BLOCK_SIZE;
    wind_printf("complete:");
    print_copy_percents(0,1,0);
    for(i = 0;i < blocks;i ++)
    {    
        for(times = 0;times < 3;times ++)
        {
            addr = src->addr + i * BLOCK_SIZE;
            if(i >= blocks - 1)
            {
                for(j = 0;j < BLOCK_SIZE;j ++)
                    buff[j] = 0;
            }
            len = read_block(src->type,src->memidx,addr,buff,1);
            if(len > 0)
                break;
        }
        if(times >= 3)
        {
            sys_warn("read block 0x%x,lenth %d failed.",addr,BLOCK_SIZE);
            dest->status = MEM_ERROR;
            return -1;
        }

        for(times = 0;times < 3;times ++)
        {
            addr = dest->addr + i * BLOCK_SIZE;
            len = write_block(dest->type,dest->memidx,addr,buff,1);
            if(len > 0)
                break;
        }
        if(times >= 3)
        {
            sys_warn("read block 0x%x,lenth %d failed.",addr,BLOCK_SIZE);
            dest->status = MEM_ERROR;
            return -1;
        }
        print_copy_percents(i,blocks,1);
        feed_watchdog();
    }
    print_copy_percents(i,blocks,1);
    wind_printf("\r\n");

    dest->datalen = src->datalen;
    dest->crc = src->crc;
    dest->status = MEM_NORMAL;

    sys_debug("copy data OK."); 
    return 0;    
}

#ifdef __cplusplus
}
#endif
