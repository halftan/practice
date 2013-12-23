// =====================================================================================
// 
//       Filename:  more.c
// 
//    Description:  
// 
//        Version:  
//        Created:  
//       Revision:  
//       Compiler:  
// 
//         Author:  Lsy_333
// 
// =====================================================================================

/*  more.c - version of more 
 *  read and print 24 lines then pause for a few special commands
 *  feature of version : reads from /dev/tty for commands
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PAGE_LEN    24
#define LINE_LEN    512

void do_more( FILE* );
int see_more( FILE* );

int read_size , total_size, percent;

int 
main( int argc, char* argv[] )
{
    FILE* fp;
    if ( argc == 1 )
            do_more( stdin );
    else
    {   
        /* 打开／dev／tty作为输入终端，并且控制属性为不用回车 */
        system( "stty -F /dev/tty cbreak" );
        while( --argc )
            if ( ( fp = fopen( * ++argv, "r" ) ) != NULL ) 
            {
                do_more( fp );
                fclose( fp );
            } 
            else    exit( 1 );
        /* 恢复终端属性为需要回车 */
        system( "stty -F /dev/tty -cbreak" );
    }
    return 0;                
}


// ===  FUNCTION  ======================================================================
//         Name:  do_more
//  Description:  read PAGE_LINE lines, then call see_more() for further instructions
// =====================================================================================
void
do_more( FILE* fp )
{
    char line[LINE_LEN];
    int num_of_lines = 0;
    int see_more( FILE* );
    int reply;
    FILE* fp_tty;
    
    if ( fp != stdin )
    {
        fseek( fp, 0, SEEK_END );
        total_size = ftell( fp );
        fseek( fp, 0, SEEK_SET );
    }


    if ( ( fp_tty = fopen( "/dev/tty", "r" ) ) == NULL )
        exit( 1 );
    while ( fgets( line, LINE_LEN, fp ) )
    {
        read_size += strlen( line ); 
        if ( num_of_lines == PAGE_LEN ) 
        {
            reply = see_more( fp_tty );
            if ( reply == 0 )   break;
            num_of_lines -= reply;
        }
        if ( fputs( line, stdout ) == EOF )
            exit( 1 );
        num_of_lines++;        
    }
}

// ===  FUNCTION  ======================================================================
//         Name:  see_more
//  Description:  print message, wait for response, return # of lines to advance
//                q means no, space means yes, CR means one line
// =====================================================================================
int
see_more( FILE* cmd )
{
    int c;
    percent = read_size * 100 / total_size;
    printf( "\033[7m--%d/%d-(%d%)-[more]--\033[0m", read_size, total_size, percent );
    while( ( c = getc( cmd ) ) != EOF  )
    {
        if( c== 'q' )   /* 退出 */   
        {    
    printf( "\033[D\033[2K" );
            return 0;
        }   
        if ( c == ' ' ) /* */
        {
            printf( "\033[A\033[2K\033[B" );
            return PAGE_LEN;
        }
        if ( c == '\n' ) /* 删除上一行显示信息并返回要显示下一行 */
        {
            printf( "\033[A\033[2K" );
            return 1;
        }
    }
    return 0;
}

