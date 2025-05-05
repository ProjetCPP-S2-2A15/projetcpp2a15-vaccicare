#!/bin/bash

# Change this to where you actually placed poppler
POPLER_PATH="C:\Users\USER-PC\Desktop\VacciCare\popler\bin"

# Use Unix-style path for checking
case ":$PATH:" in
  *":$POPLER_PATH:"*) 
    echo "Poppler path already in PATH."
    ;;
  *)
    echo "Adding Poppler path to PATH..."
    
    # Append to bash config file
    if [ -f "$HOME/.bashrc" ]; then
        echo "export PATH=\"\$PATH:$POPLER_PATH\"" >> "$HOME/.bashrc"
        echo "Added to .bashrc"
    elif [ -f "$HOME/.bash_profile" ]; then
        echo "export PATH=\"\$PATH:$POPLER_PATH\"" >> "$HOME/.bash_profile"
        echo "Added to .bash_profile"
    else
        echo "No bash profile found. Creating .bashrc"
        echo "export PATH=\"\$PATH:$POPLER_PATH\"" > "$HOME/.bashrc"
    fi

    echo "✅ Done. Restart your terminal or run 'source ~/.bashrc'"
    ;;
esac
