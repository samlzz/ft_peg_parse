#!/usr/bin/env bash

# ================================================================
# Vendoring script for ft_log
# ================================================================

set -euo pipefail
IFS=$'\n\t'

REPO_URL="https://github.com/samlzz/ft_peg_parse.git"
TARGET_DIR="."
LIB_NAME="ftpp"

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
validate_target_directory() {
    if [[ ! -d "$TARGET_DIR" ]]; then
        print_error "'$TARGET_DIR/' directory not found. Please create it before running this script."
        return 1
    fi
}

# ================================================================
# Remove existing vendored copy
# ================================================================
remove_existing_library() {
    local lib_path="$TARGET_DIR/$LIB_NAME"

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
    local dest_path="$TARGET_DIR/$LIB_NAME"

    print_info "Cloning $REPO_URL into '$dest_path' ..."
    if ! git clone "$REPO_URL" "$dest_path" --quiet; then
        print_error "Failed to clone repository from $REPO_URL"
        return 1
    fi
}

# ================================================================
# Remove metadata
# ================================================================
remove_metadata() {
    local target="$1"
    local confirm="$2"
    local resp

    if [[ ! -e "$target" ]]; then
        return 0
    fi

    if [[ "$confirm" == "true" ]]; then
        printf "Delete '%s' (y/N): " "$target"
        read -r resp
        resp=${resp//[[:space:]]/}
        resp=${resp,,}
        if [[ ! "$resp" =~ ^(y|yes)$ ]]; then
            return 0
        fi
    fi

    rm -rf -- "$target" || {
        print_error "Failed to remove '$target'"
        return 1
    }
}

# ================================================================
# Main execution flow
# ================================================================
main() {
    validate_target_directory || return 1
    remove_existing_library || return 1
    clone_repository || return 1
    remove_metadata "$TARGET_DIR/$LIB_NAME/.git" 'true' || return 1
    remove_metadata "$TARGET_DIR/$LIB_NAME/sync_ftpp.sh" 'true' || return 1
    print_info "'$LIB_NAME' successfully installed in '$TARGET_DIR/$LIB_NAME'."
}

main "$@"

