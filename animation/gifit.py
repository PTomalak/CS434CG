import os
from PIL import Image

def create_gif_from_bmps(directory="./bmp", output_file="output.gif"):

  bmp_files = [f for f in os.listdir(directory) if f.endswith('.bmp')]

  bmp_files.sort()
  images = []
  for bmp_file in bmp_files:
    file_path = os.path.join(directory, bmp_file)
    img = Image.open(file_path)
    images.append(img)

  reversed_images = images + images[::-1]
  reversed_images[0].save(output_file, save_all=True, append_images=reversed_images[1:], loop=0, duration=100)
  print("GIF created successfully as '{}'.".format(output_file))

if __name__ == "__main__":
  create_gif_from_bmps()
