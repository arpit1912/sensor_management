#!/bin/sh
rm -rf xinu.tar
tar -czvf xinu.tar ./xinu/
sshpass -p "xinurocks" ssh -P xinu@127.0.0.1 'rm -rf xinu*'
sshpass -p "xinurocks" scp -P 3022 ./xinu.tar xinu@127.0.0.1:~/.
sshpass -p "xinurocks" ssh -P xinu@127.0.0.1 'tar -xvf xinu.tar'