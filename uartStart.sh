#!/bin/bash

sleep 4

if [[ $(/bin/ss -natp) = *\"mpd\"* ]] \
  && [[ $(/bin/ss -natp) = *upmpdcli* ]] \
  && [[ $(/bin/ss -natp) = *raat_app* ]] ; then
  
  if [[ $(/sbin/ip neighbor show) = *REACHABLE* ]] \
    || [[ $(/sbin/ip neighbor show) = *STALE* ]] \
    || [[ $(/sbin/ip neighbor show) = *DELAY* ]] ; then
    echo "launching reporter program" && /root/UARTReporter/a.out
  
  elif [[ $(/sbin/ip neighbor show) != *REACHABLE* ]] \
    && [[ $(/sbin/ip neighbor show) != *STALE* ]] \
    && [[ $(/sbin/ip neighbor show) != *DELAY* ]] ; then
    echo "no network connection" && (exit 1)
  
  fi

elif [[ $(/bin/ss -natp) != *\"mpd\"* ]] \
  && [[ $(/bin/ss -natp) != *upmpdcli* ]] \
  && [[ $(/bin/ss -natp) != *raat_app* ]] ; then
  echo "mpd, upmpdcli, and raat_app have not started" && (exit 1)

elif [[ $(/bin/ss -natp) != *\"mpd\"* ]] \
  && [[ $(/bin/ss -natp) != *upmpdcli* ]] \
  && [[ $(/bin/ss -natp) = *raat_app* ]] ; then
  echo "mpd and upmpdcli have not started" && (exit 1)

elif [[ $(/bin/ss -natp) != *\"mpd\"* ]] \
  && [[ $(/bin/ss -natp) = *upmpdcli* ]] \
  && [[ $(/bin/ss -natp) != *raat_app* ]] ; then
  echo "mpd and raat_app have not started" && (exit 1)

elif [[ $(/bin/ss -natp) = *\"mpd\"* ]] \
  && [[ $(/bin/ss -natp) != *upmpdcli* ]] \
  && [[ $(/bin/ss -natp) != *raat_app* ]] ; then
  echo "upmpdcli and raat_app have not started" && (exit 1)

elif [[ $(/bin/ss -natp) != *\"mpd\"* ]] \
  && [[ $(/bin/ss -natp) = *upmpdcli* ]] \
  && [[ $(/bin/ss -natp) = *raat_app* ]] ; then
  echo "mpd has not started" && (exit 1)

elif [[ $(/bin/ss -natp) = *\"mpd\"* ]] \
  && [[ $(/bin/ss -natp) != *upmpdcli* ]] \
  && [[ $(/bin/ss -natp) = *raat_app* ]] ; then
  echo "upmpdcli has not successfully started" && (exit 1)

elif [[ $(/bin/ss -natp) = *\"mpd\"* ]] \
  && [[ $(/bin/ss -natp) = *upmpdcli* ]] \
  && [[ $(/bin/ss -natp) != *raat_app* ]] ; then
  echo "raat_app has not successfully started" && (exit 1)

else
  echo "something unexpected has gone wrong" && (exit 1)

fi

