#!/system/bin/sh

FILES="
/data/adb/modules/UnlimitedPhotosStorage/verify.sh
/data/adb/modules/UnlimitedPhotosStorage/hash
/data/adb/modules/UnlimitedPhotosStorage/customize.sh
"
# Cleanup 
for file in $FILES; do
    if [ -e "$file" ]; then
        rm -f "$file"
    fi
done

# Check if Magisk exists
if [ -d "/data/adb/magisk" ]; then
    # Check if denylist is enabled
    if magisk --denylist status; then
        # Remove Google Photos from denylist if present
        magisk --denylist rm com.google.android.apps.photos 2>/dev/null
    else
        echo "Denylist is not enabled."
    fi

else
    echo "Skipped denylist, Magisk not detected."
fi