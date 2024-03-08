#!/bin/bash

check_python() {
	if ! command -v python3 &> /dev/null
	then
		echo "Python could not be found"
		echo "Attempting to install Python3..."
		sudo apt update && sudo apt-get install python3
	else
		echo "Python3 is already installed"
	fi
}

check_venv() {
	if ! python3 -m venv --help &> /dev/null; then
		echo "venv is not installed. Installing..."
		sudo apt install python3-venv
	else
		echo "venv is already installed"
	fi
}

setup_env() {
	if [ ! -d "env" ]; then
		echo "Creating virtual environment..."
		python3 -m venv env
		echo "Virtual environment created."
	else
		echo "Virtual environment already exists."
	fi

	source env/bin/activate

	echo "Installing packages..."
	pip install numpy 

	echo "Environment setup complete."
}

check_python
check_venv
setup_env

