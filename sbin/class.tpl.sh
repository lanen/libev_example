#!/bin/bash

name=$1

echo "class $name {"
echo ""
echo "public:"
echo -e "\t $name();"
echo -e "\t ~$name();"
echo ""
echo "};"
