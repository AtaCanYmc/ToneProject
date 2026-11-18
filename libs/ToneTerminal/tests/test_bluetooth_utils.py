import unittest
from unittest.mock import patch, MagicMock
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from bluetooth_utils import handle_notification, find_notify_uuid


class TestBluetoothUtils(unittest.TestCase):

    def setUp(self):
        self.mock_characteristic = MagicMock()
        self.mock_characteristic.uuid = "abcdefab-1234-1234-1234-abcdefabcdef"

    @patch("bluetooth_utils.set_volume_mac")
    @patch("bluetooth_utils.operating_system", "Darwin")
    def test_handle_notification_valid_volume(self, mock_set_vol):
        payload = b'{"mode": "Volume", "value": "78"}'
        handle_notification(self.mock_characteristic, payload)
        mock_set_vol.assert_called_once_with(78)

    @patch("bluetooth_utils.set_volume_mac")
    @patch("bluetooth_utils.operating_system", "Darwin")
    def test_handle_notification_clamps_value(self, mock_set_vol):
        payload = b'{"mode": "Volume", "value": "180"}'
        handle_notification(self.mock_characteristic, payload)
        mock_set_vol.assert_called_once_with(100)

    @patch("bluetooth_utils.set_volume_mac")
    @patch("bluetooth_utils.operating_system", "Darwin")
    def test_handle_notification_eq_mode_does_not_alter_master_volume(self, mock_set_vol):
        # Bass mode update should not trigger set_volume_mac
        payload = b'{"mode": "Bass", "value": "45"}'
        handle_notification(self.mock_characteristic, payload)
        mock_set_vol.assert_not_called()

    @patch("bluetooth_utils.set_volume_mac")
    def test_handle_notification_malformed_json_graceful(self, mock_set_vol):
        payload = b'{"mode": "Volume", broken'
        # Should catch error without raising
        handle_notification(self.mock_characteristic, payload)
        mock_set_vol.assert_not_called()

    @patch("bluetooth_utils.set_volume_mac")
    def test_handle_notification_empty_data(self, mock_set_vol):
        handle_notification(self.mock_characteristic, b"")
        mock_set_vol.assert_not_called()

    @patch("bluetooth_utils.set_volume_mac")
    def test_handle_notification_missing_value_key(self, mock_set_vol):
        payload = b'{"mode": "Volume"}'
        handle_notification(self.mock_characteristic, payload)
        mock_set_vol.assert_not_called()

    def test_find_notify_uuid(self):
        char_write = MagicMock()
        char_write.properties = ["write"]

        char_notify = MagicMock()
        char_notify.properties = ["read", "notify"]

        service = MagicMock()
        service.characteristics = [char_write, char_notify]

        found = find_notify_uuid([service])
        self.assertEqual(found, char_notify)


if __name__ == "__main__":
    unittest.main()
