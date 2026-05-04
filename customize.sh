# Module and log directory paths
MODDIR="${0%/*}"
MEOW="/data/adb/modules/UnlimitedPhotosStorage"
SRC="/data/adb/modules_update/UnlimitedPhotosStorage/module.prop"
DEST="$MEOW/module.prop"
PKG="com.google.android.apps.photos"
TIMEOUT=15

# Verify module integrity
check_integrity() {
    echo "========================================="
    echo "              Meow Installer    "
    echo "========================================="
    echo " ✦ Verifying Module Integrity    "
    
    if [ -n "$ZIPFILE" ] && [ -f "$ZIPFILE" ]; then
        if [ -f "$MODPATH/verify.sh" ]; then
            if sh "$MODPATH/verify.sh"; then
                echo " ✦ Module integrity verified." > /dev/null 2>&1
            else
                echo " ✘ Module integrity check failed!"
                exit 1
            fi
        else
            echo " ✘ Missing verification script!"
            exit 1
        fi
    fi
}

# Create necessary directories if missing
prepare_directories() {
    echo " ✦ Preparing Required Directories  "
    [ ! -d "/data/adb/modules/UnlimitedPhotosStorage" ] && mkdir -p "/data/adb/modules/UnlimitedPhotosStorage"
    [ ! -f "$SRC" ] && return 1
}

# Handle module prop file
handle_module_props() {
    echo " ✦ Handling Module Properties "
    touch "$MEOW/update"
    cp "$SRC" "$DEST"
}

# Release the source
release_source() {
    [ -f "/data/adb/Box-Brain/meow" ] && return 0
    nohup am start -a android.intent.action.VIEW -d "https://t.me/MeowRedirect" > /dev/null 2>&1 &
}

display_footer() {
    echo "_________________________________________"
    echo " "
    echo "             Installation Completed "
    echo "    This module was released by 𝗠𝗘𝗢𝗪 𝗗𝗨𝗠𝗣"
    echo " "
    echo " "
    echo " "
}

# Let bro decide whether he wants to clear data automatically or manually 
get_key() {
    local key=""
    local tmpfile=/tmp/.getevent_$$

    # Start getevent in background
    ( timeout $TIMEOUT getevent -lqc 1 2>/dev/null > "$tmpfile" ) &
    local pid=$!

    # Wait for process to complete or timeout
    wait $pid 2>/dev/null

    # Check what we got
    if [ -f "$tmpfile" ]; then
        local event=$(grep -E "KEY_(VOLUME|POWER)" "$tmpfile" | grep "DOWN" | awk '{print $(NF-1)}')
        case "$event" in
            *VOLUMEUP*) key="UP" ;;
            *VOLUMEDOWN*) key="DOWN" ;;
            *POWER*) key="POWER" ;;
        esac
        rm -f "$tmpfile"
    fi

    if [ -z "$key" ]; then
        key="TIMEOUT"
        # Kill any leftover getevent
        killall -9 getevent 2>/dev/null
    fi

    echo "$key"
}

clear_data() {
    if pm list packages | grep -q "$PKG"; then
        echo "   "
        echo "   It is mandatory to clear data of"
        echo "   G-Photos app before rebooting"
        echo "   "
        echo "   Proceed??"
        echo "   Volume DOWN  = YES (recommended)"
        echo "   Volume UP / Touch = NO (don't do it)"
        echo "   Timeout: ${TIMEOUT}s"
        echo "   "

        local key=$(get_key)

        case "$key" in
            DOWN)
                echo " ✦ Clearing G-Photos data"
                su -c "pm clear $PKG"
                ;;
            *)
                echo " ✦ Skipped"
                ;;
        esac
    fi
}

# Main installation flow
install_module() {
    check_integrity
    prepare_directories
    handle_module_props
    clear_data
    display_footer
    release_source
}

for i in /system/product/etc/sysconfig/*; do
    file=$i
    file=${file/\/system\/product\/etc\/sysconfig\//}
    if [ ! -z "$(grep PIXEL_2020_ $i)" ] || [ ! -z "$(grep PIXEL_2021_ $i)" ] || [ ! -z "$(grep PIXEL_2019_PRELOAD $i)" ] || [ ! -z "$(grep PIXEL_2018_PRELOAD $i)" ] || [ ! -z "$(grep PIXEL_2017_PRELOAD $i)" ] || [ ! -z "$(grep PIXEL_2022_ $i)" ]; then
        [ ! -f $MODPATH/system/product/etc/sysconfig/$file ] && cat /system/product/etc/sysconfig/$file | grep -v PIXEL_2020_ | grep -v PIXEL_2021_ | grep -v PIXEL_2022_ | grep -v PIXEL_2018_PRELOAD | grep -v PIXEL_2019_PRELOAD >$MODPATH/system/product/etc/sysconfig/$file
    fi
done
for i in /system/etc/sysconfig/*; do
    file=$i
    file=${file/\/system\/etc\/sysconfig\//}
    if [ ! -z "$(grep PIXEL_2020_ $i)" ] || [ ! -z "$(grep PIXEL_2021_ $i)" ] || [ ! -z "$(grep PIXEL_2019_PRELOAD $i)" ] || [ ! -z "$(grep PIXEL_2018_PRELOAD $i)" ] || [ ! -z "$(grep PIXEL_2017_PRELOAD $i)" ] || [ ! -z "$(grep PIXEL_2022_ $i)" ]; then
        [ ! -f $MODPATH/system/product/etc/sysconfig/$file ] && cat /system/etc/sysconfig/$file | grep -v PIXEL_2020_ | grep -v PIXEL_2021_ | grep -v PIXEL_2022_ | grep -v PIXEL_2018_PRELOAD | grep -v PIXEL_2019_PRELOAD | grep -v PIXEL_2017_PRELOAD >$MODPATH/system/etc/sysconfig/$file
    fi
done
if [ -d /data/adb/modules/UnlimitedPhotosStorage/system/product/etc/sysconfig ]; then
    for i in /data/adb/modules/UnlimitedPhotosStorage/system/product/etc/sysconfig/*; do
        file=$i
        file=${file/\/data\/adb\/modules\/UnlimitedPhotosStorage\/system\/product\/etc\/sysconfig\//}
        if [ ! -f $MODPATH/system/product/etc/sysconfig/$file ]; then
            cp -f /data/adb/modules/UnlimitedPhotosStorage/system/product/etc/sysconfig/$file $MODPATH/system/product/etc/sysconfig/$file
        fi
    done
fi
if [ -d /data/adb/modules/UnlimitedPhotosStorage/system/etc/sysconfig ]; then
    for i in /data/adb/modules/UnlimitedPhotosStorage/system/etc/sysconfig/*; do
        file=$i
        file=${file/\/data\/adb\/modules\/UnlimitedPhotosStorage\/system\/etc\/sysconfig\//}
        if [ ! -f $MODPATH/system/etc/sysconfig/$file ]; then
            cp -f /data/adb/modules/UnlimitedPhotosStorage/system/etc/sysconfig/$file $MODPATH/system/etc/sysconfig/$file
        fi
    done
fi

# Initialise 
install_module
