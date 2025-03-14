#!/bin/bash

# dynamically get path
SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

# path to compiled cpp scripts
IMX_CONVERTER="$SCRIPT_DIR/imx_converter"

if [ ! -f "$IMX_CONVERTER" ]; then
    echo "Error: imx_convert binary not found!"
    exit 1
fi

if [ "$#" -ne 3 ] || [ "$1" != "convert" ]; then
    echo "Usage: imx convert <input_file> <output_file>"
    exit 
fi

"$IMX_CONVERTER" "$@"