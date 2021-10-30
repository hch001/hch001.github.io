#!/bin/zsh
source ~/.bash_profile
redis-server &
mongod --dbpath /usr/local/var/mongo --logpath /usr/local/var/log/mongo.log --auth &
activemq console &
