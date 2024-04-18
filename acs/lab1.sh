#!/bin/bash

# Дата
date=$(date)

# Имя учетной записи
username=$(whoami)

# Доменное имя ПК
hostname=$(hostname)

# Процессор
cpu_model=$(lscpu | grep "Model name" | cut -d ":" -f 2 | sed 's/^[ \t]*//')
cpu_architecture=$(lscpu | grep "Architecture" | cut -d ":" -f 2 | sed 's/^[ \t]*//')
cpu_max_freq=$(lscpu | grep "Max CPU frequency" | cut -d ":" -f 2 | sed 's/^[ \t]*//')
cpu_cur_freq=$(lscpu | grep "CPU MHz" | cut -d ":" -f 2 | sed 's/^[ \t]*//')
cpu_cores=$(lscpu | grep "Core(s) per socket" | cut -d ":" -f 2 | sed 's/^[ \t]*//')
cpu_threads=$(lscpu | grep "Thread(s) per core" | cut -d ":" -f 2 | sed 's/^[ \t]*//')
cpu_load=$(top -bn1 | grep "Cpu(s)" | awk '{print $2 + $4}')

# Оперативная память
# mem_info=$(free -h)
mem_cache_l1=$(lscpu | grep "L1d cache" | cut -d ":" -f 2 | sed 's/^[ \t]*//')
mem_cache_l2=$(lscpu | grep "L2 cache" | cut -d ":" -f 2 | sed 's/^[ \t]*//')
mem_cache_l3=$(lscpu | grep "L3 cache" | cut -d ":" -f 2 | sed 's/^[ \t]*//')
total_mem=$(grep MemTotal /proc/meminfo | sed 's/^[^0-9]*\([0-9]\)/\1/')
free_mem=$(grep MemFree /proc/meminfo | sed 's/^[^0-9]*\([0-9]\)/\1/')


echo "Дата: $date"
echo "Имя учетной записи: $username"
echo "Доменное имя ПК: $hostname"
echo "Процессор:"
echo "  Модель – $cpu_model"
echo "  Архитектура – $cpu_architecture"
echo "  Тактовая частота максимальная – $cpu_max_freq MHz"
echo "  Тактовая частота текущая (используемая) – $cpu_cur_freq MHz"
echo "  Количество ядер – $cpu_cores"
echo "  Количество потоков на одно ядро – $cpu_threads"
echo "  Загрузка процессора – $cpu_load%"
echo "Оперативная память:"
# echo "$mem_info"
echo "Cache L1 – $mem_cache_l1"
echo "Cache L2 – $mem_cache_l2"
echo "Cache L3 – $mem_cache_l3"
echo "Всего – $total_mem"
echo "Свободно – $free_mem"



# Вывод характеристик жесткого диска
echo "Жесткий диск:"
echo "Всего - $(df -h --total | awk '/total/ {print $2}')"
echo "Доступно - $(df -h --total | awk '/total/ {print $4}')"
echo "Количество разделов - $(df -h --total | awk '/^dev/ {print $1}' | wc -l)"
echo "Разделы:"
df -h --output=size,avail,target | awk '{print "  ", $1, "-", $2, "-", $3}'

# Вывод характеристик SWAP
echo "SWAP всего - $(free -h | grep 'Swap:' | awk '{print $2}')"
echo "SWAP доступно - $(free -h | grep 'Swap:' | awk '{print $4}')"


# Вывод характеристик сетевых интерфейсов
echo "Сетевые интерфейсы:"
echo "Количество сетевых интерфейсов - $(ip link show | grep -c '^[0-9]')"
echo "Интерфейсы:"
ip -brief address show | awk '{print "  Имя:", $1, "MAC:", $4, "IP:", $3}'

ip -brief address show | while read -r line; do
  interface_name=$(echo $line | awk '{print $1}')
  echo "  Имя: $interface_name"
  echo "    Максимальная скорость соединения: $(ethtool $interface_name 2>/dev/null | grep "Supported ports" -A 1 | tail -n 1 | awk '{print $NF}')"
  echo "    Фактическая скорость соединения: $(ethtool $interface_name 2>/dev/null | grep "Speed" | awk '{print $2}')"
done