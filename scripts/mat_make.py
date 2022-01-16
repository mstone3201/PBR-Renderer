import sys
from PIL import Image
import numpy as np

# make sure a material is provided
if len(sys.argv) != 2:
	print("Please enter a material name")
	sys.exit()

# for convenience
mat_path = sys.argv[1] + "/"

# basecolor - R8G8B8
# nothing to do for basecolor
try:
	base_color = Image.open(mat_path + "basecolor.png")
	base_color.save(mat_path + "bbbx.png")
	print("Generated bbbx.png")
except Exception:
	print("Invalid basecolor... Skipping...")

# normal - R8G8B8
# height - R8
# normal + height are appended
try:
	normal = Image.open(mat_path + "normal.png") # note: even if normal is in R16G16B16 format, it is still read as R8G8B8
	height = Image.open(mat_path + "height.png")
	if height.mode == "I":
		height_data = np.asarray(height)
		height = Image.fromarray(height_data >> 8) # convert from R16 to R8
		height = height.convert("L")
	normal.putalpha(height)
	normal.save(mat_path + "nnnh.png")
	print("Generated nnnh.png")
except Exception:
	print("Invalid normal/height map... Skipping...")

# roughness - R8
# metallic - R8
# ambientocclusion - R8
# roughness + metallic + ambientocclusion are appended
try:
	roughness = Image.open(mat_path + "roughness.png")
	if roughness.mode == "I":
		roughness_data = np.asarray(roughness)
		roughness = Image.fromarray(roughness_data >> 8) # convert from R16 to R8
		roughness = roughness.convert("L")
	metallic = Image.open(mat_path + "metallic.png")
	if metallic.mode == "I":
		metallic_data = np.asarray(metallic)
		metallic = Image.fromarray(metallic_data >> 8) # convert from R16 to R8
		metallic = metallic.convert("L")
	ambient_occlusion = Image.open(mat_path + "ambientocclusion.png")
	if ambient_occlusion.mode == "I":
		ambient_occlusion_data = np.asarray(ambient_occlusion)
		ambient_occlusion = Image.fromarray(ambient_occlusion_data >> 8) # convert from R16 to R8
		ambient_occlusion = ambient_occlusion.convert("L")
	rmax = Image.merge("RGB", (roughness.getchannel(0), metallic.getchannel(0), ambient_occlusion.getchannel(0)))
	rmax.save(mat_path + "rmax.png")
	print("Generated rmax.png")
except Exception:
	print("Invalid roughness/metallic/ambientocclusion map... Skipping...")