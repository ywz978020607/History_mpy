"""
MicroPython LM75A Digital Temperature Sensor
https://github.com/mcauser/micropython-lm75a

MIT License
Copyright (c) 2019 Mike Causer

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

__version__ = '0.0.3'

# registers
_LM75A_TEMP  = const(0x00) # Temperature register (r) 16-bit
_LM75A_CONF  = const(0x01) # Configuration register (r/w) 8-bit, default: enabled, comparator, os active low, single fault
_LM75A_THYST = const(0x02) # Hysteresis register (r/w) 16-bit, default: 75°C
_LM75A_TOS   = const(0x03) # Overtemperature shutdown register (r/w) 16-bit, default: 80°C

class LM75A:
	def __init__(self, i2c, address=0x48):
		self._i2c = i2c
		self._address = address # 0x48-0x4F
		self._config = 0x00
		self._buf1 = bytearray(1)
		self._buf2 = bytearray(2)
		self.check()
		self.config()

	def check(self):
		if self._i2c.scan().count(self._address) == 0:
			raise OSError('LM75A not found at I2C address {:#x}'.format(self._address))

	def config(self, shutdown=None, os_mode=None, os_polarity=None, os_fault_queue=None):
		if shutdown is not None:
			self._config = (self._config & ~1) | (shutdown & 1)

		if os_mode is not None:
			self._config = (self._config & ~2) | ((os_mode << 1) & 2)

		if os_polarity is not None:
			self._config = (self._config & ~4) | ((os_polarity << 2) & 4)

		if os_fault_queue is not None:
			self._config = (self._config & ~24) | ((os_fault_queue << 3) & 24)

		self._buf1[0] = self._config
		self._i2c.writeto_mem(self._address, _LM75A_CONF, self._buf1)

	def temp(self):
		self._i2c.readfrom_mem_into(self._address, _LM75A_TEMP, self._buf2)
		val = (self._buf2[0] << 3) | (self._buf2[1] >> 5)
		return self._twos_comp(val, 11) * 0.125

	def tos(self, temp):
		self._temp_to_9bit_reg(temp)
		self._i2c.writeto_mem(self._address, _LM75A_TOS, self._buf2)

	def thyst(self, temp):
		self._temp_to_9bit_reg(temp)
		self._i2c.writeto_mem(self._address, _LM75A_THYST, self._buf2)

	def _twos_comp(self, val, bits):
		mask = 2 ** (bits - 1)
		return -(val & mask) + (val & ~mask)

	def _rev_twos_comp(self, val, bits):
		return val & ((1 << bits) -1)

	def _temp_to_9bit_reg(self, temp):
		val = self._rev_twos_comp(int(temp / 0.5), 9)
		self._buf2[0] = val >> 1
		self._buf2[1] = val << 7
