
#include "user_interface.h"
#include "AS608_Func.h"


//用户接口
char *item_content(Item *item)
{
    static char content[12];
    if(item->pointer!=VALUE) sprintf(content,"%s",item->name);
    else if(item->func.data.type==INT) sprintf(content,"%s:%d",item->name,*((int*)item->func.data.value));
    else if(item->func.data.type==FLOAT) sprintf(content,"%s:%.2f",item->name,*((float*)item->func.data.value));
    return content;
}

Page page1;
Page page2;
Page page3;
Page steerpid;
Item item1,item2,item3,item4,item5,item6,item7,item8;
int sss;
void init_ui()
{
    set_show_area(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);//设置显示区域
   
    init_page(&page1,"设置");//初始化页面 
    set_item_indexpage(&item1,&page2,"添加用户");
    set_item_indexpage(&item2,&page3,"查看用户");    
    set_item_function(&item3,del_FR,"删除用户");
    set_item_function(&item4,NULL,"更改密码");
    page_add_item(&page1,&item1);
    page_add_item(&page1,&item2);
	page_add_item(&page1,&item3);
	page_add_item(&page1,&item4); 

    init_page(&page2,"添加用户"); //页面2
    set_item_function(&item5,add_admin_people,"添加管理员用户");
    set_item_function(&item6,add_common_people,"添加普通用户");
    page_add_item(&page2,&item5);
    page_add_item(&page2,&item6);


    init_page(&page3,"查看用户"); //页面2
    set_item_function(&item7,show_admin_people,"查看管理员用户");
    set_item_function(&item8,show_common_people,"查看普通用户");
    page_add_item(&page3,&item7);
    page_add_item(&page3,&item8);

    set_base_page(&page1);//设置主页
}

