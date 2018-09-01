#!/bin/bash

IFS_FILE=/etc/control.ifs
COUNTER_FILE=/etc/control.count
COUNT_LIMIT=120
CURRENT_DATE=$(date '+%Y-%m-%d')
CURRENT_TIME=$(date '+%H:%M:%S')
IFCONFIG=/sbin/ifconfig
CUT=/usr/bin/cut
GREP=/bin/grep
PING=/bin/ping

function save_ifs {
  echo save_ifs
  ${IFCONFIG} | ${CUT} -d":" -f 1 | ${CUT} -d" " -f 1 | ${GREP} -v ^$ | ${GREP} -v ^lo$ > ${IFS_FILE}
}

function change_ifs {
  echo change_ifs
  while read ni; do
    ${IFCONFIG} ${ni} $1
  done < ${IFS_FILE}
}

function add_count {
  echo add_count
  echo ${CURRENT_DATE} ${CURRENT_TIME} >> ${COUNTER_FILE}
}

function verifica_data {
  echo verifica_data
  last_date=$(tail -1 ${COUNTER_FILE} | cut -d' ' -f1)
  if [ "${last_date}" != "${CURRENT_DATE}" ]; then
    change_ifs up
  else
    verifica_count off
  fi
}

function verifica_count {
  echo verifica_count
  [ "$1" = "on" ] && save_ifs
  current_count=$(strings ${COUNTER_FILE} | grep ${CURRENT_DATE} | wc -l)
  if [ ${current_count} -ge ${COUNT_LIMIT} ]; then
    change_ifs down
    shutdown -h now
  else
    change_ifs up
    [ "$1" = "on" ] && add_count
  fi
}

function main {
  echo main
  ${PING} -c 1 8.8.8.8 &> /dev/null
  result=$?
  [ ${result} -eq 0 ] && verifica_count on
  [ ${result} -gt 0 ] && verifica_data
}

main
