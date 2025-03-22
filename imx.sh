#!/bin/bash

# Paths to compiled binaries
IMX_CONVERTER="/home/ashish/development/imx/imx_converter"
IMX_COMPRESSOR="/home/ashish/development/imx/imx_compressor"
IMX_VIEWER="/home/ashish/development/imx/imx_viewer"

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
elif [ "$1" == "view" ]; then
    if [ ! -f "$IMX_VIEWER" ]; then
        echo "Error: imx_viewer binary not found!"
        exit 1
    fi
    "$IMX_VIEWER" "$2"
else
    echo "Usage: imx <convert|compress|view> [options] <input_file> <output_file>"
    exit 1
fi