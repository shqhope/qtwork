#include "commonfunctions.h"
#include <stdio.h>   /*标准输入输出的定义*/
#include <errno.h>  /*错误号定义*/
#include <sys/stat.h>
#include <fcntl.h>  /*文件控制定义*/

#include <termios.h>    /*PPSIX 终端控制定义*/
#include <stdlib.h> /*标准函数库定义*/
#include <sys/types.h>
#include <unistd.h> /*UNIX 标准函数定义*/
#include <string.h>
#include <ctype.h>

CommonFunctions::CommonFunctions()
{
}
