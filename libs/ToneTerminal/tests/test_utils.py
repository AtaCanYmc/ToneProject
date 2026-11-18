import unittest
from unittest.mock import patch, MagicMock
import sys
import os

# Add parent directory to sys.path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from utils import log, get_timestamp, set_volume_mac, get_volume_mac, get_volume_data_as_int


class TestToneUtils(unittest.TestCase):

    def test_get_timestamp_format(self):
        ts = get_timestamp()
        self.assertRegex(ts, r"^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$")

    @patch("subprocess.run")
    def test_set_volume_mac_clamps_bounds(self, mock_run):
        # Above 100 should clamp to 100
        set_volume_mac(150)
        mock_run.assert_called_with(
            ["osascript", "-e", "set volume output volume 100"],
            check=True,
            capture_output=True
        )

        # Below 0 should clamp to 0
        set_volume_mac(-25)
        mock_run.assert_called_with(
            ["osascript", "-e", "set volume output volume 0"],
            check=True,
            capture_output=True
        )

        # Within bounds
        set_volume_mac(72)
        mock_run.assert_called_with(
            ["osascript", "-e", "set volume output volume 72"],
            check=True,
            capture_output=True
        )

    @patch("subprocess.run")
    def test_get_volume_mac_success(self, mock_run):
        mock_res = MagicMock()
        mock_res.returncode = 0
        mock_res.stdout = "64\n"
        mock_run.return_value = mock_res

        val = get_volume_mac()
        self.assertEqual(val, 64)

    @patch("subprocess.run")
    def test_get_volume_mac_fallback_on_error(self, mock_run):
        mock_run.side_effect = Exception("osascript process timeout")
        val = get_volume_mac()
        self.assertEqual(val, 50)

    @patch("utils.get_volume_mac")
    def test_get_volume_data_as_int_darwin(self, mock_mac):
        mock_mac.return_value = 80
        val = get_volume_data_as_int("Darwin")
        self.assertEqual(val, 80)
        mock_mac.assert_called_once()

    def test_get_volume_data_as_int_unknown_os_fallback(self):
        val = get_volume_data_as_int("FreeBSD")
        self.assertEqual(val, 50)


if __name__ == "__main__":
    unittest.main()
