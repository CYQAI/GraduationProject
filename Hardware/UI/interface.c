
#include "user_interface.h"

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
    init_page(&page1,"中景");//初始化页面 
    // set_item_indexpage(&item1,&page2,"set pids");
    set_item_indexpage(&item2,NULL,"set speed");
    set_item_indexpage(&item3,NULL,"show img");
    set_item_indexpage(&item4,NULL,"item4");
    set_item_indexpage(&item5,NULL,"item5");
    set_item_indexpage(&item6,NULL,"item6");
	
    // init_page(&page2,"PID"); //页面2




    // init_page(&page3,"page3");//页面3

    // set_item_indexpage(&item7,&page3,"item7");
    // page_add_item(&page2,&item7);

    // set_item_value_int(&item8,&sss,1,"num");
    // page_add_item(&page3,&item8);

    // //给page添加item选项
    // page_add_item(&page1,&item1);
	page_add_item(&page1,&item2);
    page_add_item(&page1,&item3);
	page_add_item(&page1,&item4);
    page_add_item(&page1,&item5);
    page_add_item(&page1,&item6);
    set_base_page(&page1);//设置主页
}

