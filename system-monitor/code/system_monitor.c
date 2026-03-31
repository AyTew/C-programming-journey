#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

char *progress_bar(double val, int max) {
    static char bar[33];
    bar[0]='[';
    bar[31]=']';

    int fill = (int)(10*val)/max;
    for(int i=1; i<=30; i=i+3) {
        if(i<=fill) {
            bar[i] = '\xe2';
            bar[i+1] = '\x96';
            bar[i+2] = '\x88';
        }else {
            bar[i] = '\xe2';
            bar[i+1] = '\x96';
            bar[i+2] = '\x91';
        }
    }
    bar[32]='\0';
    return bar;
}

int main() {
    const int memory_conversion_factor = 1024;
    char buffer[1024];
    char *keys[5] = {"Free Memory", "Active Memory", "Inactive Memory", "Speculative Memory", "Throttled Memory"};
    char *headers[3] = {"╔══════════════════════════════════╗", "║      SYSTEM MONITOR v1.0         ║", "╚══════════════════════════════════╝"};
    int free_len[5];


    for(int i=0; i<5; i++) {
        free_len[i]=36-(strlen(keys[i])+1); //+1 for ':'
    }

    while(1){
        for(int i=0; i<3; i++) {
            printf("%s\n", headers[i]);
        }
        FILE *pointer = popen("vm_stat", "r");
        // fgets(buffer, 1024, stdin);
        fgets(buffer, 1024, pointer);
        for(int i=0; i<5; i++) {
            fgets(buffer, 1024, pointer);
            int dot = strchr(buffer, '.')-buffer;
            
            int pos2=dot;
            while(buffer[pos2]!=' ') {
                pos2--;
            }

            pos2++;

            char val[dot-pos2+1];

            for(int j=pos2; j<dot; j++){
                val[j-pos2]=buffer[j];
            }
            double mem_kb = (double)atoi(val)*16384/memory_conversion_factor;
            double mem_mb = (double)mem_kb/memory_conversion_factor;
            double mem_gb = 0.0;

            if(mem_mb/memory_conversion_factor >= 1) {
                mem_gb = mem_mb/1024;
            }
            val[dot-pos2] = '\0';

            int total_mem = 8*memory_conversion_factor*memory_conversion_factor;

            char *bar = progress_bar(mem_kb, total_mem);

            if(mem_gb>0) {
                printf("%s: %.3f GB %s\n", keys[i], mem_gb, bar);
            } else {
                printf("%s: %.3f MB %s\n", keys[i], mem_mb, bar);
            }
        }
        pclose(pointer);
        sleep(2);
        int clear_status = system("clear");
        if(clear_status==-1) {
            printf("SOME ERROR OCCURED IN SYSTEM CALL FOR CLEAR");
            break;
        }
    }
}
