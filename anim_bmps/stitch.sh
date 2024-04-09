#!/bin/bash
cd pngs

# Use ffmpeg to convert BMP files to MP4
ffmpeg -framerate 30 -i filename_%04d.png -c:v libx264 -r 30 -pix_fmt yuv420p -crf 18 -preset slow ../output.mp4

# Step 1: Reverse the order of the input images
ffmpeg -framerate 30 -i filename_%04d.png -vf reverse reversed_images_%04d.png

# Step 2: Render the reversed images to a video
ffmpeg -framerate 30 -i reversed_images_%04d.png -c:v libx264 -r 30 -pix_fmt yuv420p -crf 18 -preset slow ../reversed_output.mp4

cd ..

# Step 3: Stitch the two videos together
ffmpeg -f concat -i list.txt -c copy combined_output.mp4





