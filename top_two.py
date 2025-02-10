def get_top_two(list):
    winner = run_tournament([[c] for c in list])
    second = winner[1]
    for victim in winner[2:]:
        if victim > second:
            second = victim

    return winner[0], second


def run_tournament(tributes):
    if len(tributes) <= 1:
        return tributes[0]

    winners = []
    if len(tributes) % 2 == 1:
        winners.append(tributes.pop())

    for i in range(0, len(tributes), 2):
        if tributes[i][0] > tributes[i + 1][0]:
            tributes[i].append(tributes[i + 1][0])
            winners.append(tributes[i])
        else:
            tributes[i + 1].append(tributes[i][0])
            winners.append(tributes[i + 1])

    return run_tournament(winners)


print(get_top_two([8, 7, 6, 5, 4, 3, 2, 8]))
