# zsh icns.sh yourimage.png icon.incs
# 网上找的代码
PNG=$1
OUT=$2
if [ -z "$PNG" ] || [ -z "$OUT" ];then
	echo "[error]:please input png filename and out filename!"
	exit
fi
if [ ! -d "icons.iconset" ];then
	echo "[info]:create dir icons.iconset"
	mkdir icons.iconset
else
	echo "[error]:icons.iconset dir already exists"
exit
fi

sips -z 16 16 $PNG --out icons.iconset/icon_16x16.png
sips -z 32 32 $PNG --out icons.iconset/icon_16x16@2x.png
sips -z 32 32 $PNG --out icons.iconset/icon_32x32.png
sips -z 64 64 $PNG --out icons.iconset/icon_32x32@2x.png
sips -z 64 64 $PNG --out icons.iconset/icon_64x64.png
sips -z 128 128 $PNG --out icons.iconset/icon_64x64@2x.png
sips -z 128 128 $PNG --out icons.iconset/icon_128x128.png
sips -z 256 256 $PNG --out icons.iconset/icon_128x128@2x.png
sips -z 256 256 $PNG --out icons.iconset/icon_256x256.png
sips -z 512 512 $PNG --out icons.iconset/icon_256x256@2x.png
sips -z 512 512 $PNG --out icons.iconset/icon_512x512.png
sips -z 1024 1024 $PNG --out icons.iconset/icon_512x512@2x.png
iconutil -c icns icons.iconset -o $OUT
rm -rf icons.iconset
echo "[info]:done!"