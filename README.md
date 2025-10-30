# Simple C Configuration File Parser

## Overview

This repository contains a simple configuration file parser written in **C**. The program reads key-value pairs from a plain text configuration file, validates that all **required configurations** are present (based on a separate file), and outputs the loaded settings into a neatly formatted **JSON file**.

It's designed for simple, line-by-line parsing of configuration settings formatted as `key=value`.

---

## Features

* **Config Parsing:** Reads and parses configuration files with entries in the format `key=value`.
* **Comment & Blank Line Handling:** Ignores lines starting with `#` (comments) and blank lines.
* **Required Config Check:** Reads a list of keys from `required_configs.txt` and reports to `stderr` if any are missing from the parsed configuration.
* **JSON Output:** Converts the loaded configurations into a valid JSON object (`output.json`).