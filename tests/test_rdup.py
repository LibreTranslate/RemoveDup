import pytest
from removedup import rdup
import os

def test_rdup():
    cwd = os.path.dirname(__file__)

    src, tgt, removed = rdup(os.path.join(cwd, "data", "src.txt"), os.path.join(cwd, "data", "tgt.txt"))
    assert removed == 1
