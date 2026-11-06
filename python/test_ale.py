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

from AlwaysLessEffort import AlwaysLessEffort, AleTolerances, STATUS, isOutsideBand, existanceCheckNotPassed, computeApproxDays

def test_not_accepted_conditions():
    ale = AlwaysLessEffort(1, 50, 15)
    assert ale.status == STATUS[2]
    ale = AlwaysLessEffort(15, 1, 15)
    assert ale.status == STATUS[2]
    ale = AlwaysLessEffort(15, 50, 0)
    assert ale.status == STATUS[2]
    ale = AlwaysLessEffort(1e7, 50, 15)
    assert ale.status == STATUS[2]
    ale = AlwaysLessEffort(15, 50, 1e23)
    assert ale.status == STATUS[2]
    ale = AlwaysLessEffort(50, 1e13, 15)
    assert ale.status == STATUS[0]
    ale = AlwaysLessEffort(50, 1e14, 15)
    assert ale.status == STATUS[2]
    ale = AlwaysLessEffort(1e6, 10, 15)
    assert ale.status == STATUS[1]
    ale = AlwaysLessEffort(1e7, 10, 15)
    assert ale.status == STATUS[2]

def test_approx_days():
    ad, oid, N = computeApproxDays(13.5)
    assert ad == 13.5 and oid is False and N == 27
    ad, oid, N = computeApproxDays(12)
    assert ad == 12 and oid is True and N == 12
    ad, oid, N = computeApproxDays(12.3)
    assert ad == 12.5 and oid is False and N == 25
    ad, oid, N = computeApproxDays(12.7)
    assert ad == 12.5 and oid is False and N == 25
    ad, oid, N = computeApproxDays(13.15)
    assert ad == 13 and oid is True and N == 13
    ad, _, _ = computeApproxDays(12.249999)
    assert ad == 12
    ad, _, _ = computeApproxDays(12.250001)
    assert ad == 12.5
    ad, _, _ = computeApproxDays(12.75)
    assert ad == 13
    


def test_random_standard_cases():
    nd, np, desired_rate = 1.5, 30, 15
    while nd < 50 and np < 500 and desired_rate < 150:
        standardIteration(nd, np, desired_rate)
        nd += 1.5
        standardIteration(nd, np, desired_rate)
        np += 17.5
        standardIteration(nd, np, desired_rate)
        desired_rate += 10
    tol = AleTolerances(sensRate = 0.001, sensExactPhases = 0.001, sensApproxSinglePhase = 0.01, sensApproxTotPhases = 0.1, maxErrIter = 5)
    ale = AlwaysLessEffort(15.5, 347.5, 90, tol)
    assert ale.status != STATUS[2]


def standardIteration(nd, np, desired_rate):
    assert not existanceCheckNotPassed(nd, 1e6, (desired_rate + 100.0) / 100.0, 1e20, np, 0.01)
    ale = AlwaysLessEffort(nd, np, desired_rate)
    assert ale.status != STATUS[2]
    assert not isOutsideBand(ale.totExactPhases, ale.np, ale.tol.sensExactPhases)
    assert not isOutsideBand(ale.rate_first_last, ale.rp, ale.tol.sensRate)