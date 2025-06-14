"""Common types for audio plugins."""

from __future__ import annotations

from typing import Mapping, Sequence, Protocol, Generic, TypeVar

import equinox as eqx
from jaxtyping import Array, Float

# Audio buffer type.
# Samples are ordered so that:
#   - buffer[0] is the earliest played sample.
#   - buffer[1] is the second earliest played sample.
#   - buffer[-1] is the latest.
Buffer = Float[Array, "BufferSize"]

# Re-exported for convenience.
Module = eqx.Module

# Type of the plugin state.
PluginState = TypeVar("PluginState", bound=Module)


class Plugin(Protocol, Generic[PluginState]):
    """Audio plugin with state `PluginState`."""

    @property
    def input_buffer_names(self) -> Sequence[str]:
        """Names of all input buffers."""

    def init(self, inputs: Mapping[str, Buffer],
             sample_rate: Float[Array, ""]) -> PluginState:
        """Initializes the plugin, given the first batch of samples. Called once."""

    def update(
        self,
        state: PluginState,
        inputs: Mapping[str, Buffer],
        sample_rate: Float[Array, ""],
    ) -> tuple[PluginState, Mapping[str, Buffer]]:
        """Processes a frame of audio data.
        
        Args:
          state: State of the plugin.
          inputs: Input buffers.
          sample_rate: Sample rate.
  
        Returns:
          New state and output buffers.
        """


class PluginConfig(Protocol):
    """Configuration for a plugin."""

    def make(self) -> Plugin:
        """Creates a configured plugin."""
