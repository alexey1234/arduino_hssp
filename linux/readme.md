# PRE

If you have troubles with compile psocdude branch by Dirk Petrautzki  https://github.com/miracoli/psocdude I put compilled binaries for x86 and x64 UBUNTUs

# HOWTO

Just download binary.zip and psocdude.conf.zip

Unzip its

Check  unzipped bynary with command ldd how it will work on your computer

If all libraries presents you can simple copy files to folders
```
cp psocdude /usr/local/bin/
```
```
cp psocdude.conf /usr/local/etc/
```
Check permisions for bynary 
```
 ls -l /usr/local/bin/psocdude
-rwxr-xr-x 1 root root 512060 מרץ 23 10:22 /usr/local/bin/psocdude
```
for config
```
 ls -l /usr/local/etc/psocdude.conf
-rw-rw-rw- 1 root root 33952 מרץ 23 15:34 /usr/local/etc/psocdude.conf
```

and make bynary executable + change owner if need

MUST WORK
 ## End PRE

My linux x86 computer 
```
 proger@proger-945GCM-S2L:~$ hostnamectl
   Static hostname: proger-945GCM-S2L
         Icon name: computer-desktop
           Chassis: desktop
        Machine ID: 8589b2f7c5aa4765a6ce561b59f6b923
           Boot ID: a55257a2231e4b47beed1c94c6a0ec6c
  Operating System: Ubuntu 18.04.6 LTS
            Kernel: Linux 4.15.0-213-generic
      Architecture: x86

```
My Linux x64 computer
```
alexey@alexey-NUC14RVH-B:~$ hostnamectl
 Static hostname: alexey-NUC14RVH-B
       Icon name: computer-desktop
         Chassis: desktop 🖥️
      Machine ID: b6cede8aef754511813cc9aacaeef8e3
         Boot ID: a52db3b2bb8b45c58826d8f83b03c50a
Operating System: Ubuntu 24.04.2 LTS              
          Kernel: Linux 6.11.0-21-generic
    Architecture: x86-64
 Hardware Vendor: ASUSTeK COMPUTER INC.
  Hardware Model: NUC14RVH-B
Firmware Version: RVRPLR30.0035.2024.0814.1037
   Firmware Date: Wed 2024-08-14
    Firmware Age: 8month 3d
```

psocdude x86 use libraries 	  
```	  
 ldd psocdude
        linux-gate.so.1 (0xb7efe000)
        libelf.so.1 => /usr/lib/i386-linux-gnu/libelf.so.1 (0xb7e92000)
        libpthread.so.0 => /lib/i386-linux-gnu/libpthread.so.0 (0xb7e72000)
        libc.so.6 => /lib/i386-linux-gnu/libc.so.6 (0xb7c96000)
        libz.so.1 => /lib/i386-linux-gnu/libz.so.1 (0xb7c77000)
        /lib/ld-linux.so.2 (0xb7f00000)
```
psocdude x64 use libraries
```
ldd psocdude
	 linux-vdso.so.1 (0x0000746178b65000)
	 libelf.so.1 => /lib/x86_64-linux-gnu/libelf.so.1 (0x0000746178afe000)
	 libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x0000746178800000)
 	libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x0000746178ae2000)
	 libzstd.so.1 => /lib/x86_64-linux-gnu/libzstd.so.1 (0x0000746178a28000)
	 /lib64/ld-linux-x86-64.so.2 (0x0000746178b67000)

```

In 2 machines I cannot compile libhid, but I think it no need for Arduino|Seeeduino nano
```diff
Configuration summary:

DO HAVE    libelf
DO HAVE    libusb
DO HAVE    libusb_1_0
DO HAVE    libftdi1
DON'T HAVE libftdi
-DON'T HAVE libhid
DO HAVE    pthread
DISABLED   doc
ENABLED    parport
DISABLED   linuxgpio

```

