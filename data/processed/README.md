# data/processed/

This directory holds derived results produced by the scripts
in `analysis/` from the primary data in `data/raw/`.

Files here are never edited by hand. Every file is always regenerable by rerunning the corresponding script (e.g. `analysis/parser.py` with
`--csv`/`--markdown`) against the raw data it was derived from.
