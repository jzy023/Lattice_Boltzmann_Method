#!/usr/bin/python3

import os
import numpy as np
import multiprocessing
from glob import glob
from pyevtk.hl import pointsToVTK


def to_vtk(arr, i):
    pointsToVTK(str(i), arr[:, 0], arr[:, 1], arr[:, 2], data={
        "ux": arr[:, 3],
        "uy": arr[:, 4],
        "umag": arr[:, 5]
    })


def start(file):
    arr = np.loadtxt(file + ".csv", dtype=float, skiprows=1, delimiter=',')
    arr = np.array(arr, order='F')
    to_vtk(arr, file)


def create_pvd_file(files):
    with open("output.pvd", "w") as pvd:
        pvd.write('<?xml version="1.0"?>\n')
        pvd.write('<VTKFile type="Collection" version="0.1" byte_order="LittleEndian">\n')
        pvd.write('  <Collection>\n')
        
        for timestep, file in enumerate(sorted(files)):
            pvd.write(f'    <DataSet timestep="{timestep}" file="{file}.vtu"/>\n')
        
        pvd.write('  </Collection>\n')
        pvd.write('</VTKFile>\n')


def main():
    files = [os.path.splitext(f)[0] for f in glob("*.csv")]
    print(f"Number of CSV files found: {len(files)}")
    
    # Generate .vtu files in parallel
    pool = multiprocessing.Pool(10)
    pool.map(start, files)
    
    # Create the .pvd file for time series
    create_pvd_file(files)


if __name__ == "__main__":
    main()
