#!/bin/bash
printf "\n-------------\nVariable Decs:\n-------------\n\n"
set -v
KEYA=11
KEYB=14
PLAINTEXT='testfile_affine_cipher.txt'
set +v
OUTPUT=$PLAINTEXT'-encrypted-with-'$KEYA-$KEYB'.txt'
DEOUTPUT=$PLAINTEXT'-decrypted-with-'$KEYA-$KEYB'.txt'
printf "\n"
gcc -ansi -pedantic -Wall -o affine affine.c
printf "\nRunning encryption:\n./affine -e $PLAINTEXT $OUTPUT $KEYA $KEYB
\n"
./affine -e $PLAINTEXT $OUTPUT $KEYA $KEYB
printf "\n\nRunning decryption:\n./affine -d $OUTPUT $DEOUTPUT $KEYA $KEYB\n"
./affine -d $OUTPUT $DEOUTPUT $KEYA $KEYB
printf "\nRunning diff analysis:\ndiff -s $PLAINTEXT $DEOUTPUT\n"
diff -s $PLAINTEXT $DEOUTPUT
printf "\n-----------\nAll Done! HD plz\n-----------\n\n"
