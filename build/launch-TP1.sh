#!/bin/sh
bindir=$(pwd)
cd /home/sylvain/Documents/MoteurDeJeu/Minecraft/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/sylvain/Documents/MoteurDeJeu/Minecraft/build/TP1 
	else
		"/home/sylvain/Documents/MoteurDeJeu/Minecraft/build/TP1"  
	fi
else
	"/home/sylvain/Documents/MoteurDeJeu/Minecraft/build/TP1"  
fi
