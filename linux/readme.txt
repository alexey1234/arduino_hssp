My linux x86 computer 

 proger@proger-945GCM-S2L:~$ hostnamectl
   Static hostname: proger-945GCM-S2L
         Icon name: computer-desktop
           Chassis: desktop
        Machine ID: 8589b2f7c5aa4765a6ce561b59f6b923
           Boot ID: a55257a2231e4b47beed1c94c6a0ec6c
  Operating System: Ubuntu 18.04.6 LTS
            Kernel: Linux 4.15.0-213-generic
      Architecture: x86

psocdude use libraries 	  
	  
 ldd /usr/local/bin/psocdude
        linux-gate.so.1 (0xb7efe000)
        libelf.so.1 => /usr/lib/i386-linux-gnu/libelf.so.1 (0xb7e92000)
        libpthread.so.0 => /lib/i386-linux-gnu/libpthread.so.0 (0xb7e72000)
        libc.so.6 => /lib/i386-linux-gnu/libc.so.6 (0xb7c96000)
        libz.so.1 => /lib/i386-linux-gnu/libz.so.1 (0xb7c77000)
        /lib/ld-linux.so.2 (0xb7f00000)

