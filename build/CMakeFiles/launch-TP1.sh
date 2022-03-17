#!/bin/sh
bindir=$(pwd)
cd /home/pups/Documents/Minecraft/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/pups/Documents/Minecraft/build/TP1 
	else
		"/home/pups/Documents/Minecraft/build/TP1"  
	fi
else
	"/home/pups/Documents/Minecraft/build/TP1"  
fi
