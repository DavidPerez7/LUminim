#!/bin/bash

set -e

print_success() {
    echo "[+] $1"
}

print_error() {
    echo "[-] $1" >&2
}

print_info() {
    echo "[*] $1"
}

detect_os() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        OS=$ID
        OS_VERSION=$VERSION_ID
    else
        print_error "Cannot detect operating system"
        exit 1
    fi
    
    case "$OS" in
        ubuntu)
            DISTRO_TYPE="debian"
            DISTRO_NAME="Ubuntu"
            ;;
        debian)
            DISTRO_TYPE="debian"
            DISTRO_NAME="Debian"
            ;;
        linuxmint)
            DISTRO_TYPE="debian"
            DISTRO_NAME="Linux Mint"
            ;;
        parrot)
            DISTRO_TYPE="debian"
            DISTRO_NAME="Parrot OS"
            ;;
        void)
            DISTRO_TYPE="void"
            DISTRO_NAME="Void Linux"
            ;;
        *)
            print_error "Unsupported OS: $OS"
            echo "Supported distributions: Ubuntu, Debian, Linux Mint, Parrot OS, Void Linux"
            exit 1
            ;;
    esac
}

install_deps_debian() {
    print_info "Installing dependencies ($DISTRO_NAME)"
    
    if ! command -v apt-get &> /dev/null; then
        print_error "apt-get not found"
        exit 1
    fi
    
    if ! command -v sudo &> /dev/null; then
        print_error "sudo not installed"
        exit 1
    fi
    
    print_info "Updating package lists..."
    if ! sudo apt-get update -qq; then
        print_error "Failed to update packages"
        exit 1
    fi
    
    print_info "Checking for broken dependencies..."
    if ! sudo apt-get check -qq; then
        print_error "Broken dependencies detected. Run: sudo apt --fix-broken install"
        exit 1
    fi
    
    DEBIAN_PACKAGES="build-essential qt5-qmake qtbase5-dev qtdeclarative5-dev \
libqt5svg5-dev libx11-dev libxcb-util0-dev libxcb-icccm4-dev \
libxcb-image0-dev libxcb-xfixes0-dev libxcb-xkb-dev \
libxkbcommon-x11-dev libfluxbox-dev git pkg-config libssl-dev"
    
    print_info "Installing required packages..."
    if ! sudo apt-get install -y $DEBIAN_PACKAGES; then
        print_error "Failed to install dependencies"
        exit 1
    fi
    
    print_success "Dependencies installed"
}

install_deps_void() {
    print_info "Installing dependencies ($DISTRO_NAME)"
    
    if ! command -v xbps-install &> /dev/null; then
        print_error "xbps-install not found"
        exit 1
    fi
    
    if ! command -v sudo &> /dev/null; then
        print_error "sudo not installed"
        exit 1
    fi
    
    print_info "Syncing repositories..."
    if ! sudo xbps-install -S; then
        print_error "Failed to sync repositories"
        exit 1
    fi
    
    print_info "Updating packages..."
    if ! sudo xbps-install -u xbps; then
        print_error "Failed to update xbps"
        exit 1
    fi
    
    VOID_PACKAGES="base-devel qt5-base qt5-declarative qt5-svg \
libxcb-devel xcb-util-devel xcb-util-icccm-devel \
xcb-util-image-devel xcb-util-xfixes-devel xkeyboard-config-devel \
fluxbox-devel git pkg-config openssl-devel"
    
    print_info "Installing required packages..."
    if ! sudo xbps-install -y $VOID_PACKAGES; then
        print_error "Failed to install dependencies"
        exit 1
    fi
    
    print_success "Dependencies installed"
}

build_lumina() {
    REPO_DIR="$(dirname "$0")"
    BUILD_DIR="$REPO_DIR/build"
    
    if [ ! -d "$REPO_DIR/src-qt5" ]; then
        print_error "src-qt5 directory not found"
        exit 1
    fi
    
    if [ ! -f "$REPO_DIR/src-qt5/src-qt5.pro" ]; then
        print_error "src-qt5/src-qt5.pro not found"
        exit 1
    fi
    
    if [ ! -x "$(command -v qmake)" ]; then
        print_error "qmake not found. Install Qt5 development tools"
        exit 1
    fi
    
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR" || exit 1
    
    print_info "Running qmake..."
    if ! qmake ../src-qt5/src-qt5.pro; then
        print_error "qmake configuration failed"
        exit 1
    fi
    
    CORES=$(nproc 2>/dev/null || echo 1)
    print_info "Compiling (using $CORES cores)..."
    if ! make -j "$CORES"; then
        print_error "Build failed"
        exit 1
    fi
    
    print_success "Build completed"
}

install_system() {
    BUILD_DIR="$(dirname "$0")/build"
    
    if [ ! -d "$BUILD_DIR" ]; then
        print_error "Build directory not found"
        exit 1
    fi
    
    if [ ! -f "$BUILD_DIR/Makefile" ]; then
        print_error "Makefile not found. Run build first"
        exit 1
    fi
    
    cd "$BUILD_DIR" || exit 1
    
    if ! command -v sudo &> /dev/null; then
        print_error "sudo not installed"
        exit 1
    fi
    
    print_info "Installing (requires sudo)..."
    if ! sudo make install; then
        print_error "Installation failed"
        exit 1
    fi
    
    print_success "Installation completed"
}

verify_installation() {
    if command -v lumina-desktop &> /dev/null; then
        print_success "lumina-desktop verified in PATH"
        return 0
    else
        print_error "lumina-desktop not found in PATH"
        return 1
    fi
}

main() {
    print_info "Detecting system..."
    
    detect_os
    print_info "Detected: $DISTRO_NAME (ID: $OS)"
    
    case "$DISTRO_TYPE" in
        debian)
            install_deps_debian
            ;;
        void)
            install_deps_void
            ;;
        *)
            print_error "Unsupported distribution type: $DISTRO_TYPE"
            exit 1
            ;;
    esac
    
    build_lumina
    install_system
    
    print_info "Verifying installation..."
    if verify_installation; then
        print_success "Installation complete. Select LUminim at your login screen."
    else
        print_error "Verification failed"
        exit 1
    fi
}

trap 'print_error "Installation aborted"; exit 1' ERR INT TERM
main "$@"
