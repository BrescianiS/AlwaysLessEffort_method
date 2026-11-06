#
#  Copyright 2026 -  Samuel Bresciani - bresciani.app@gmail.com
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

import math

STATUS = ('OK', 'WARNING', 'ERROR')

def isOutsideBand(value, target, tolerance):
    return value <= (target - tolerance) or value >= (target + tolerance)

def existanceCheckNotPassed(nd, MAX_DAYS, rp, COMP_LIMIT, np, sensApproxSinglePhase):
    if not ( nd >= 1.25 and nd <= MAX_DAYS and rp > 1 and rp < COMP_LIMIT and np > 1 and np < COMP_LIMIT and sensApproxSinglePhase > 0 ) :
        return True        
    check_q = 2.0 * np / nd
    check_m = 2.0 * np / (nd * (nd - 1.0))
    if ( check_q >= COMP_LIMIT or check_m >= COMP_LIMIT ) :
        return True

def computeApproxDays(nd):
    half_days = math.floor(nd * 2.0 + 0.5) 
    approx_days = half_days / 2.0
    only_integer_days = (half_days % 2) == 0
    N = half_days // 2 if only_integer_days else half_days
    return approx_days, only_integer_days, N

class AleTolerances:
    def __init__(self, sensRate = 0.0001, sensExactPhases = 0.01, sensApproxSinglePhase = 0.01, sensApproxTotPhases = 0.1, maxErrIter = 2):        
        # Desired sensibility for the rate of work
        self.sensRate = sensRate
        # Desired sensibility for the exact number of phases
        self.sensExactPhases = sensExactPhases
        # Rounding step of the approximated phases of a single day
        self.sensApproxSinglePhase = sensApproxSinglePhase
        # Desired sensibility for the approximated total number of phases
        self.sensApproxTotPhases = sensApproxTotPhases
        # Maximum number of times that the correction iterations for the error happens
        self.maxErrIter = maxErrIter

    
class AlwaysLessEffort():
    _m , _q = 0.0, 0.0
    # computational limit
    COMP_LIMIT = 1e20
    # maximum number of days: N vectors elements are allocated and N is stored in an int
    MAX_DAYS = 1e6

    def __init__(self, nd, np, desired_rate, ale_tolerances = None):        
        self.nd = nd
        self.np = np
        # here the percentage is converted in actual rate for the computation
        self.rp = (desired_rate + 100.0) / 100.0        
        self.tol = ale_tolerances if ale_tolerances is not None else AleTolerances()        
        # error Status
        self.status = STATUS[0]
        # to distinguish the two cases of nd as integer or not
        self.only_integer_days = True
        self.approx_days = 0.0
        # other computation variables
        self.rate_first_last, self.totApproxPhases, self.totExactPhases = 0.0, 0.0, 0.0
        self.d, self.pd, self.pdap = [], [], []
        self.wrong_rate = False
        self._m, self._q = 0.0, 0.0
        
        if existanceCheckNotPassed(self.nd, self.MAX_DAYS, self.rp, self.COMP_LIMIT, self.np, self.tol.sensApproxSinglePhase):
            self.status = STATUS[2]
            return
        
        # all conditions of existance satisfied
        self.algorithmElaboration()
        return

    def algorithmElaboration(self):
        # nd is rounded to the nearest half day: halfDays is the number of half days
        # (nd >= 1.25 is guaranteed by the constructor, so half_days >= 3)
        self.approx_days, self.only_integer_days, N = computeApproxDays(self.nd)
        self.d, self.pd, self.pdap = [], [], []
        self.parametersComputation(N)
        return
    

    # FUNCTIONS TO COMPUTE THE ALGORITHM RESULTS

    # Computation m and q parameters function
    def parametersComputation(self, N):
        # q and m parameters definition
        self._q = self.np * 2.0 * self.rp / (N * (self.rp + 1.0))
        self._m = self._q * (self.rp - 1.0) / (self.rp * (N - 1))
        # Algorithm results computation
        self.resultsComputation(N)

        # self-correction phases in case of sensibility not satisfied
        for _ in range(self.tol.maxErrIter): 
            if not ( self.status == STATUS[2] or self.wrong_rate ): return  # break and return
            # Rescale m to P and adjust q
            self._m = self._m* (self.np / self.totExactPhases)
            self._q = (self.np + self._m * (N* (N - 1) / 2.0)) / N

            mSpan = self._m * (N - 1)
            if (self._q == mSpan): return  # break and return
            R_check = self._q / (self._q - mSpan)
            # if the correction drifts the R value from desired one, more than 2 * sensRate, the correction is stopped
            if isOutsideBand(R_check, self.rp, 2.0 * self.tol.sensRate) or self._q < mSpan : return  # break and return

            self.resultsComputation(N)

        return
        
    # Results computation function
    def resultsComputation(self, N):
        self.d = list(range(1, N+1))
        self.status = STATUS[0]

        # iterations
        self.pd = [self._q - i *self._m for i in range(N)]
        self.pdap = [math.floor(0.5 + value / self.tol.sensApproxSinglePhase) * self.tol.sensApproxSinglePhase for value in self.pd]

        # Final checks of computed values near enough to desired values
        # exact phases
        self.totExactPhases = math.fsum(self.pd)
        if isOutsideBand(self.totExactPhases, self.np, self.tol.sensExactPhases):
            self.status = STATUS[2]
        else:
            # approximated phases
            self.totApproxPhases = math.fsum(self.pdap)
            if isOutsideBand(self.totApproxPhases, self.np, self.tol.sensApproxTotPhases):
                self.status = STATUS[1]            

        # rate between first and last day value
        self.rate_first_last = self.pd[0] / self.pd[-1]
        self.wrong_rate = isOutsideBand(self.rate_first_last, self.rp, self.tol.sensRate)
        return
    