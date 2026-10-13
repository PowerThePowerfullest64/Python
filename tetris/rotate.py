from surface import Surface

def rotate(piece, surface: Surface, force = False):
    if len(piece) == 0:
        return piece.copy()
    if not force and not can_rotate(piece, surface):
        return piece.copy()

    rotated_x, rotated_y = _rotated_coordinates(piece, surface)
    return (rotated_y * surface.width + rotated_x).astype(int)

def _rotated_coordinates(piece, surface: Surface):
    x = piece % surface.width
    y = piece // surface.width
    min_x = x.min()
    min_y = y.min()
    local_x = x - min_x
    local_y = y - min_y

    rotated_x = local_y.max() - local_y
    rotated_y = local_x
    return min_x + rotated_x, min_y + rotated_y

def can_rotate(piece, surface: Surface) -> bool:
    if len(piece) == 0:
        return True

    rotated_x, rotated_y = _rotated_coordinates(piece, surface)

    for x, y in zip(rotated_x, rotated_y):
        if not surface.inside(x, y):
            return False

    return True
