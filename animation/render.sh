#!/bin/bash

rm ray
cp ../exe/ray ./

rm ./json/*
rm ./bmp/*

python render.py
python gifit.py

rm ./bmp/*