#/bin/sh


tc qdisc add dev eth0 root tbf rate 1mbit latency 25ms burst 10k

python client.py