#!/bin/bash

if [ "x$1" == "x" ]
then
	echo "Usage: $0 address"
	exit 0;
fi

echo $1
curl --header "Content-Type: application/json" \
  --request POST \
  --data '{"somejson":"xyz","otherjson":"xyz"}' \
http://$1/engageprofile
