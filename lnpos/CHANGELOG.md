0.1.4
======
- Fix typo with logging 'lnurlATM' configuration to serial
- Remove unused WebServer to reduce build time, file size and installation time
- Show a little "arrow" in front of the selected menu item to avoid ambiguity when only 2 menu items are present (or when the user is color blind)
- Make "USB" indicator blue so it looks better and is easier to distinguish
- Show firmware version at boot to allow the user to easily check which version is running (handy for troubleshooting)
- Fix fiat conversion API call compatibility with LNBits
- Change "LNPoS", "Offline PoS", "OnChain" and "ATM" menu items to more explicit "Receive Online", "Receive Offline", "Receive OnChain" and "Send Offline"
