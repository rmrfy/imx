#!/bin/bash

# path to compiled cpp scripts
IMX_CONVERTER="/home/ashish/development/imx/imx_converter"
IMX_COMPRESSOR="/home/ashish/development/imx/imx_compressor"

if [ "$1" == "convert" ]; then
    if [ ! -f "$IMX_CONVERTER" ]; then
        echo "Error: imx_converter binary not found!"
        exit 1
    fi
    "$IMX_CONVERTER" "$@"
elif [ "$1" == "compress" ]; then
    if [ ! -f "$IMX_COMPRESSOR" ]; then
        echo "Error: imx_compressor binary not found!"
        exit 1
    fi
    "$IMX_COMPRESSOR" "$@"
else
    echo "Usage: imx <convert|compress> [options] <input_file> <output_file>"
    exit 1
fi