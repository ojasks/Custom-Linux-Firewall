#!/bin/bash

echo "ACK SCAN"
nmap -sA -p 80 127.0.0.1
echo "XMAS SCAN"
nmap -sX -p 80 127.0.0.1
echo "FIN SCAN"
nmap -sF -p 80 127.0.0.1
echo "SYN SCAN"
nmap -sS -p 80 127.0.0.1
echo "NULL SCAN"
nmap -sN -p 80 127.0.0.1
