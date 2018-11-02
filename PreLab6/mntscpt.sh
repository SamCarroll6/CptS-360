 #! /bin/bash
   
   if [ $# -eq 0 ]
   then
   	dd if=/dev/zero of=mydisk bs=1024 count=1440
   	mkfs -b 1024 mydisk 1440
   	sudo mount -o loop mydisk /mnt
   else
	dd if=/dev/zero of=$1 bs=1024 count=1440
        mkfs -b 1024 $1 1440
        sudo mount -o loop $1 /mnt
   fi
   (cd /mnt; sudo mkdir dir1 dir2 dir3 dir4 dir1/dir2; sudo touch file1 file2 file3 file4; ls -l;)
   sudo umount /mnt
