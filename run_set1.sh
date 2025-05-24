#!/bin/bash
#set -e

echo "Running set1_challenge1"
echo "---------OUTPUT---------"
gcc -o set1_challenge1 set1_challenge1.c lib/utils.c -I./
./set1_challenge1 49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d
rm set1_challenge1
echo "-------END OUTPUT-------"
echo ""

echo "Running set1_challenge2"
echo "---------OUTPUT---------"
gcc -o set1_challenge2 set1_challenge2.c lib/utils.c -I./
./set1_challenge2 1c0111001f010100061a024b53535009181c 686974207468652062756c6c277320657965
rm set1_challenge2
echo "-------END OUTPUT-------"
echo ""

