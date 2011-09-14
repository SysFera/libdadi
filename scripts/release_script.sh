#! /bin/sh

# @brief release script for libdadi
# @author Haïkel Guémar <haikel.guemar@gmail.com>
#

## tarball generation information
name="libdadi"
version=""
format="bz2"
commit="HEAD"

## error handling message
function usage {
    echo "Usage: $0 [-V version] [-f <compression format>] [-c <commit id>]"
    exit 1
}

## extract version
function extract_version {
    awk 'BEGIN { major=""; minor=""; patch="" } \
/set\(DADI_VERSION_MAJOR/ { sub(/\)/, "", $2); major=$2 } \
/set\(DADI_VERSION_MINOR/ { sub(/\)/, "", $2); minor=$2 } \
/set\(DADI_VERSION_PATCH/ { sub(/\)/, "", $2); patch=$2 } \
END { version =  major "." minor "." patch; print version}' CMakeLists.txt
}

## set header
function set_copyright {
    header=$(cat scripts/copyright.txt)
    for i in $(ls **/*.{hh,cc})
    do
        awk -v test="$header" '!/LICENCE/ { print $0 }; /LICENCE/ { print test }' $i > $i-tmp
        mv $i-tmp $i
    done
}

## tarball generation
function archive {
    nvr="$name-$version"
    compressor="bzip2"
    case $format in
        "zip")
            git archive --prefix="$nvr/" --format=zip $commit -o "$nvr.zip"
            return 0
            ;;
        "gz")
            compressor="gzip"
            ;;
        "xz")
            compressor="xz -c"
            ;;
        "bz2")
            ;;
        "*")
            compressor="bzip2"
            format="bz2"
            ;;
    esac
    git archive --prefix="$nvr/" --format=tar $commit | \
        $compressor >"$nvr.tar.$format"
}

## script entry point
function main {
    while getopts ":vV:f:" opt; do
        case $opt in
            v)
                echo "verbose mode set"
                ;;
            V)
                echo "version $OPTARG"
                # data validation
                version=$(echo $OPTARG | tr -cd '[[:alnum:].] [:space:]')
                ;;
            f)
                echo "format: $OPTARG"
                format=$OPTARG
                ;;
            c)
                echo "commit: $OPTARG"
                commit=$OPTARG
                ;;
            h)
                echo "[usage]"
                usage
                ;;
            \?)
                echo "Invalid option: $OPTARG"
                usage
                ;;
        esac
    done

    version=$(extract_version)
    set_copyright
    archive
}

main $@