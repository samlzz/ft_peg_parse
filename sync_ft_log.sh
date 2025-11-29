#!/usr/bin/env bash

# ================================================================
# Vendoring script for ft_log
# ================================================================

set -euo pipefail
IFS=$'\n\t'

REPO_URL="https://github.com/samlzz/ft_log.git"
VENDOR_DIR="vendor"
TARGET_DIR="include"
LIB_NAME="ft_log"

# ================================================================
# Print to stderr
# ================================================================
print_error() {
    local msg="$1"
    printf "Error: %s\n" "$msg" >&2
}

# ================================================================
# Print to stdout
# ================================================================
print_info() {
    local msg="$1"
    printf "%s\n" "$msg"
}

# ================================================================
# Validate directory existence
# ================================================================
validate_vendor_directory() {
    if [[ ! -d "$VENDOR_DIR" ]]; then
        print_error "'$VENDOR_DIR/' directory not found. Please create it before running this script."
        return 1
    fi
}

# ================================================================
# Remove existing vendored copy
# ================================================================
remove_existing_library() {
    local lib_path="$VENDOR_DIR/$LIB_NAME"

    if [[ -d "$lib_path" ]]; then
        print_info "Removing existing '$lib_path' ..."
        rm -rf "$lib_path" || {
            print_error "Failed to remove existing '$lib_path'"
            return 1
        }
    fi
}

# ================================================================
# Clone repository into vendor directory
# ================================================================
clone_repository() {
    local dest_path="$VENDOR_DIR/$LIB_NAME"

    print_info "Cloning $REPO_URL into '$dest_path' ..."
    if ! git clone "$REPO_URL" "$dest_path" --quiet; then
        print_error "Failed to clone repository from $REPO_URL"
        return 1
    fi
}

# ================================================================
# Remove .git metadata
# ================================================================
remove_git_metadata() {
    local git_dir="$VENDOR_DIR/$LIB_NAME/.git"

    if [[ -d "$git_dir" ]]; then
        print_info "delete '$git_dir/.git'"
        rm -rf "$git_dir" || {
            print_error "Failed to remove .git directory"
            return 1
        }
    fi
}

# ================================================================
# Copy public headers into include directory
# ================================================================
copy_public_headers() {
    local src_include="$VENDOR_DIR/$LIB_NAME/include"

    if [[ ! -d "$TARGET_DIR" ]]; then
        print_error "'$TARGET_DIR/' directory does not exist. Headers were not copied."
        return 1
    fi

    if [[ ! -d "$src_include" ]]; then
        print_error "No 'include/' directory found in vendored library."
        return 1
    fi

    print_info "Copying headers from '$src_include' to '$TARGET_DIR/' ..."
    cp -R "$src_include/"* "$TARGET_DIR/" || {
        print_error "Failed to copy header files."
        return 1
    }
}

# ================================================================
# Main execution flow
# ================================================================
main() {
    validate_vendor_directory || return 1
    remove_existing_library || return 1
    clone_repository || return 1
    remove_git_metadata || return 1
    copy_public_headers || return 1
    print_info "'$LIB_NAME' done."
}

main "$@"

