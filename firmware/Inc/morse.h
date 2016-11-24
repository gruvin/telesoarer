/*
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program in the file Lincese.txt.  If not, please
 * see <http://www.gnu.org/licenses/>
 */
#ifndef __MORSE_H
#define __MORSE_H

// CW dit length (arbitrary units)
#define DITLENGTH 70 // milli-seconds

// input pulse width measurement
// Timer 1 counter clock = 1MHz
#define MIDPOINT 1500 // uS
#define PULSE_TIMEOUT (23000 / 250) // 23mS, in 250uS increments

extern void morseSend(char *s);
extern int  isMorseReady();
extern void morseStop();
extern void morseStateMachine();

#endif // __MORSE_H

