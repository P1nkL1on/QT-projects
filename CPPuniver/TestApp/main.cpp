#include "cstdlib"
#include "iostream"
#include <stdio.h>
using namespace std;

// variant Number 24
// fprintf


int fprintf0 (FILE *fl, const char *format, ...){
    const char *format1 = format, *format2 = format;
    va_list vl;
    int bytesWriten = 0;
    // make a list of vatriables
    _crt_va_start(vl, format);
    while (*format2)
        if (*format2++ == '%'){
            char type = *(format2++);
            if (type == 'i'){
                int argI = _crt_va_arg(vl, int);
                int buffer[100]; int count = 0;
                while (argI && ++count){buffer[count - 1] =(argI % 10 + 48); argI /= 10; }

                for (int i = count - 1; i >=0 ; i--)
                    fputc(buffer[i], fl);
                bytesWriten += count;
            }
             if (type == 's'){
                const char* argS = _crt_va_arg(vl, char*);
                bytesWriten += fputs(argS, fl);
            }
            if (type == 'c'){
                const int argC = _crt_va_arg(vl, int);
                fputc(argC, fl);
                bytesWriten ++;
            }
            if (type == 'b'){
                int argD = _crt_va_arg(vl, int);
                bytesWriten += fputs(((argD)?"TRUE" : "FALSE"), fl);
            }

        }else{
            fputc( *(format2 - 1), fl);
            bytesWriten ++;
        }

    _crt_va_end(vl);
    return bytesWriten;
}
FILE *stream;
int main (int argc, char* args[]){

    fopen_s( &stream, "fprintf.out", "w" );
    int d = fprintf0(stream, "string - %s,\nstring - %s,\nchar - %c,\nint -  %i,\nbool - %b,\nbool - %b",
                     "haha, int", "lolno", '@', 1567, true, false, 12,13);
    d = fprintf0(stream, "okay");
    fclose( stream );
    system( "type fprintf.out" );
    return d;
}
