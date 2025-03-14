#!/bin/bash

# path to compiled cpp scripts
IMX_CONVERTER="/home/ashish/development/imx/imx_converter"

if [ ! -f "$IMX_CONVERTER" ]; then
    echo "Error: imx_converter binary not found!"
    exit 1
fi

if [ "$#" -ne 3 ] || [ "$1" != "convert" ]; then
    echo "Usage: imx convert <input_file> <output_file>"
    exit 1
fi

"$IMX_CONVERTER" "$@"